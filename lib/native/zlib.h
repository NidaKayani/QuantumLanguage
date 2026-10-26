// <zlib.h> for Quantum's native backend: zlib's streaming API over a
// whole-buffer inflate (stored / fixed / dynamic Huffman blocks; gzip, zlib
// or raw framing) and deflate (LZ77 with hash chains, dynamic Huffman).
// A stream takes all of its input before producing output; the output is
// then handed out in pieces as avail_out allows, the way zlib's is.
#pragma once
#include <string>
#include <vector>

typedef unsigned char Bytef;
typedef unsigned int uInt;
typedef unsigned long uLong;
typedef void *voidpf;

#define Z_OK 0
#define Z_STREAM_END 1
#define Z_NEED_DICT 2
#define Z_ERRNO (-1)
#define Z_STREAM_ERROR (-2)
#define Z_DATA_ERROR (-3)
#define Z_MEM_ERROR (-4)
#define Z_BUF_ERROR (-5)
#define Z_VERSION_ERROR (-6)
#define Z_NO_FLUSH 0
#define Z_PARTIAL_FLUSH 1
#define Z_SYNC_FLUSH 2
#define Z_FULL_FLUSH 3
#define Z_FINISH 4
#define Z_NO_COMPRESSION 0
#define Z_BEST_SPEED 1
#define Z_BEST_COMPRESSION 9
#define Z_DEFAULT_COMPRESSION (-1)
#define Z_FILTERED 1
#define Z_HUFFMAN_ONLY 2
#define Z_DEFAULT_STRATEGY 0
#define Z_DEFLATED 8
#define MAX_WBITS 15

struct __z_state
{
    bool deflating = false;
    int wrap = 0;    // 0 raw, 1 zlib, 2 gzip, 3 either (inflate: detect)
    int level = 6;
    std::string in;  // everything given so far
    std::string out; // everything produced
    size_t given = 0; // of out, handed to the caller
    bool finished = false;
};

struct z_stream
{
    Bytef *next_in;
    uInt avail_in;
    uLong total_in;
    Bytef *next_out;
    uInt avail_out;
    uLong total_out;
    const char *msg;
    __z_state *state;
    void *zalloc;
    void *zfree;
    void *opaque;
    int data_type;
    uLong adler;
    uLong reserved;
};
typedef z_stream *z_streamp;

namespace __zlib
{

inline unsigned crc32Update(unsigned crc, const unsigned char *p, size_t n)
{
    static unsigned table[256];
    static bool ready = false;
    if (!ready)
    {
        for (unsigned i = 0; i < 256; ++i)
        {
            unsigned c = i;
            for (int k = 0; k < 8; ++k)
                c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1;
            table[i] = c;
        }
        ready = true;
    }
    crc = ~crc;
    for (size_t i = 0; i < n; ++i)
        crc = table[(crc ^ p[i]) & 255] ^ (crc >> 8);
    return ~crc;
}

inline unsigned adler32Update(unsigned adler, const unsigned char *p, size_t n)
{
    unsigned a = adler & 0xFFFF, b = adler >> 16;
    for (size_t i = 0; i < n; ++i)
    {
        a = (a + p[i]) % 65521;
        b = (b + a) % 65521;
    }
    return (b << 16) | a;
}

// ── inflate ──────────────────────────────────────────────────────────────

struct Bits
{
    const unsigned char *p;
    size_t n, pos = 0;
    unsigned long long buf = 0;
    int count = 0;
    bool overrun = false;
    Bits(const unsigned char *data, size_t size) : p(data), n(size) {}
    int need(int k)
    {
        while (count < k)
        {
            if (pos >= n)
            {
                overrun = true;
                return 0;
            }
            buf |= (unsigned long long)p[pos++] << count;
            count += 8;
        }
        return 1;
    }
    unsigned get(int k)
    {
        if (k == 0)
            return 0;
        if (!need(k))
            return 0;
        unsigned v = (unsigned)(buf & ((1ull << k) - 1));
        buf >>= k;
        count -= k;
        return v;
    }
    void align()
    {
        int drop = count % 8;
        buf >>= drop;
        count -= drop;
    }
};

// canonical Huffman decoding: counts per length, symbols in code order
struct Huffman
{
    unsigned short count[16];
    std::vector<unsigned short> symbol;
    bool build(const unsigned char *lengths, int n)
    {
        for (int i = 0; i < 16; ++i)
            count[i] = 0;
        for (int i = 0; i < n; ++i)
            ++count[lengths[i]];
        count[0] = 0;
        int left = 1;
        for (int len = 1; len < 16; ++len)
        {
            left <<= 1;
            left -= count[len];
            if (left < 0)
                return false; // over-subscribed
        }
        unsigned short offs[16];
        offs[1] = 0;
        for (int len = 1; len < 15; ++len)
            offs[len + 1] = offs[len] + count[len];
        symbol.assign(n, 0);
        for (int i = 0; i < n; ++i)
            if (lengths[i])
                symbol[offs[lengths[i]]++] = (unsigned short)i;
        return true;
    }
    int decode(Bits &b) const
    {
        int code = 0, first = 0, index = 0;
        for (int len = 1; len < 16; ++len)
        {
            code |= (int)b.get(1);
            if (b.overrun)
                return -1;
            int c = count[len];
            if (code - c < first)
                return symbol[index + (code - first)];
            index += c;
            first += c;
            first <<= 1;
            code <<= 1;
        }
        return -2;
    }
};

static const unsigned short lenBase[29] = {3,  4,  5,  6,  7,  8,  9,  10, 11,  13,  15,  17,  19,  23, 27,
                                           31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};
static const unsigned char lenExtra[29] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2,
                                           2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
static const unsigned short distBase[30] = {1,   2,   3,   4,   5,   7,    9,    13,   17,   25,
                                            33,  49,  65,  97,  129, 193,  257,  385,  513,  769,
                                            1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577};
static const unsigned char distExtra[30] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6,
                                            6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
static const unsigned char clOrder[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

// 0: done; 1: ran out of input; -1: corrupt (msg says why)
inline int inflateRaw(Bits &b, std::string &out, const char *&msg)
{
    for (;;)
    {
        int last = (int)b.get(1);
        int type = (int)b.get(2);
        if (b.overrun)
            return 1;
        if (type == 0)
        {
            b.align();
            unsigned len = b.get(16), nlen = b.get(16);
            if (b.overrun)
                return 1;
            if ((len ^ 0xFFFF) != nlen)
            {
                msg = "invalid stored block lengths";
                return -1;
            }
            // whatever is left in the bit buffer is whole bytes now
            while (len && b.count >= 8)
            {
                out += (char)b.get(8);
                --len;
            }
            if (b.pos + len > b.n)
                return 1;
            out.append((const char *)b.p + b.pos, len);
            b.pos += len;
        }
        else if (type == 1 || type == 2)
        {
            Huffman lit, dist;
            if (type == 1)
            {
                unsigned char l[288];
                for (int i = 0; i < 144; ++i)
                    l[i] = 8;
                for (int i = 144; i < 256; ++i)
                    l[i] = 9;
                for (int i = 256; i < 280; ++i)
                    l[i] = 7;
                for (int i = 280; i < 288; ++i)
                    l[i] = 8;
                lit.build(l, 288);
                unsigned char d[30];
                for (int i = 0; i < 30; ++i)
                    d[i] = 5;
                dist.build(d, 30);
            }
            else
            {
                int nlen = (int)b.get(5) + 257, ndist = (int)b.get(5) + 1, ncode = (int)b.get(4) + 4;
                if (b.overrun)
                    return 1;
                unsigned char lengths[320];
                for (int i = 0; i < 19; ++i)
                    lengths[clOrder[i]] = 0;
                for (int i = 0; i < ncode; ++i)
                    lengths[clOrder[i]] = (unsigned char)b.get(3);
                if (b.overrun)
                    return 1;
                Huffman cl;
                if (!cl.build(lengths, 19))
                {
                    msg = "invalid code lengths set";
                    return -1;
                }
                int index = 0;
                while (index < nlen + ndist)
                {
                    int sym = cl.decode(b);
                    if (sym == -1)
                        return 1;
                    if (sym < 0)
                    {
                        msg = "invalid code lengths set";
                        return -1;
                    }
                    if (sym < 16)
                        lengths[index++] = (unsigned char)sym;
                    else
                    {
                        int len = 0, rep;
                        if (sym == 16)
                        {
                            if (index == 0)
                            {
                                msg = "invalid bit length repeat";
                                return -1;
                            }
                            len = lengths[index - 1];
                            rep = 3 + (int)b.get(2);
                        }
                        else if (sym == 17)
                            rep = 3 + (int)b.get(3);
                        else
                            rep = 11 + (int)b.get(7);
                        if (b.overrun)
                            return 1;
                        if (index + rep > nlen + ndist)
                        {
                            msg = "invalid bit length repeat";
                            return -1;
                        }
                        while (rep--)
                            lengths[index++] = (unsigned char)len;
                    }
                }
                if (!lit.build(lengths, nlen) || !dist.build(lengths + nlen, ndist))
                {
                    msg = "invalid literal/lengths set";
                    return -1;
                }
            }
            for (;;)
            {
                int sym = lit.decode(b);
                if (sym == -1)
                    return 1;
                if (sym < 0)
                {
                    msg = "invalid literal/length code";
                    return -1;
                }
                if (sym < 256)
                    out += (char)sym;
                else if (sym == 256)
                    break;
                else
                {
                    sym -= 257;
                    if (sym >= 29)
                    {
                        msg = "invalid literal/length code";
                        return -1;
                    }
                    size_t len = lenBase[sym] + b.get(lenExtra[sym]);
                    int ds = dist.decode(b);
                    if (ds == -1 || b.overrun)
                        return 1;
                    if (ds < 0 || ds >= 30)
                    {
                        msg = "invalid distance code";
                        return -1;
                    }
                    size_t d = distBase[ds] + b.get(distExtra[ds]);
                    if (b.overrun)
                        return 1;
                    if (d > out.size())
                    {
                        msg = "invalid distance too far back";
                        return -1;
                    }
                    size_t from = out.size() - d;
                    for (size_t k = 0; k < len; ++k)
                        out += out[from + k];
                }
            }
        }
        else
        {
            msg = "invalid block type";
            return -1;
        }
        if (last)
            return 0;
    }
}

// a whole stream: its framing, then the blocks, then the check value
inline int inflateAll(const std::string &in, int wrap, std::string &out, const char *&msg)
{
    const unsigned char *p = (const unsigned char *)in.data();
    size_t n = in.size(), start = 0;
    if (wrap == 3)
        wrap = n >= 2 && p[0] == 0x1F && p[1] == 0x8B ? 2 : 1;
    if (wrap == 2)
    {
        if (n < 10)
            return 1;
        if (p[0] != 0x1F || p[1] != 0x8B || p[2] != 8)
        {
            msg = "incorrect header check";
            return -1;
        }
        int flags = p[3];
        start = 10;
        if (flags & 4) // extra field
        {
            if (n < start + 2)
                return 1;
            start += 2 + (p[start] | (p[start + 1] << 8));
        }
        if (flags & 8) // file name
        {
            while (start < n && p[start])
                ++start;
            ++start;
        }
        if (flags & 16) // comment
        {
            while (start < n && p[start])
                ++start;
            ++start;
        }
        if (flags & 2)
            start += 2;
        if (start > n)
            return 1;
    }
    else if (wrap == 1)
    {
        if (n < 2)
            return 1;
        if ((p[0] & 15) != 8 || ((p[0] << 8) | p[1]) % 31 != 0)
        {
            msg = "incorrect header check";
            return -1;
        }
        start = 2;
    }
    Bits b(p + start, n - start);
    int r = inflateRaw(b, out, msg);
    if (r != 0)
        return r;
    b.align();
    size_t tail = start + b.pos - b.count / 8;
    if (wrap == 2)
    {
        if (n < tail + 8)
            return 1;
        unsigned crc = p[tail] | (p[tail + 1] << 8) | (p[tail + 2] << 16) | ((unsigned)p[tail + 3] << 24);
        if (crc != crc32Update(0, (const unsigned char *)out.data(), out.size()))
        {
            msg = "incorrect data check";
            return -1;
        }
    }
    else if (wrap == 1)
    {
        if (n < tail + 4)
            return 1;
        unsigned ad = ((unsigned)p[tail] << 24) | (p[tail + 1] << 16) | (p[tail + 2] << 8) | p[tail + 3];
        if (ad != adler32Update(1, (const unsigned char *)out.data(), out.size()))
        {
            msg = "incorrect data check";
            return -1;
        }
    }
    return 0;
}

// ── deflate ──────────────────────────────────────────────────────────────

struct BitWriter
{
    std::string &out;
    unsigned long long buf = 0;
    int count = 0;
    explicit BitWriter(std::string &o) : out(o) {}
    void put(unsigned v, int k)
    {
        buf |= (unsigned long long)v << count;
        count += k;
        while (count >= 8)
        {
            out += (char)(buf & 255);
            buf >>= 8;
            count -= 8;
        }
    }
    // Huffman codes go most significant bit first
    void code(unsigned c, int len)
    {
        unsigned r = 0;
        for (int i = 0; i < len; ++i)
            r |= ((c >> i) & 1) << (len - 1 - i);
        put(r, len);
    }
    void flush()
    {
        if (count > 0)
            out += (char)(buf & 255);
        buf = 0;
        count = 0;
    }
};

// code lengths (at most `limit` bits) for the frequencies
inline void huffmanLengths(const std::vector<unsigned> &freqIn, int limit, std::vector<unsigned char> &len)
{
    int n = (int)freqIn.size();
    len.assign(n, 0);
    std::vector<unsigned> freq = freqIn;
    for (;;)
    {
        std::vector<int> used;
        for (int i = 0; i < n; ++i)
            if (freq[i])
                used.push_back(i);
        if (used.empty())
            return;
        if (used.size() == 1)
        {
            len[used[0]] = 1;
            return;
        }
        // nodes: leaves, then merged parents; repeatedly join the two lightest
        std::vector<unsigned long long> weight;
        std::vector<int> parent;
        std::vector<int> live;
        for (int s : used)
        {
            weight.push_back(freq[s]);
            parent.push_back(-1);
            live.push_back((int)weight.size() - 1);
        }
        while (live.size() > 1)
        {
            int a = 0, b = 1;
            if (weight[live[b]] < weight[live[a]])
            {
                a = 1;
                b = 0;
            }
            for (int i = 2; i < (int)live.size(); ++i)
            {
                if (weight[live[i]] < weight[live[a]])
                {
                    b = a;
                    a = i;
                }
                else if (weight[live[i]] < weight[live[b]])
                    b = i;
            }
            int na = live[a], nb = live[b];
            weight.push_back(weight[na] + weight[nb]);
            parent.push_back(-1);
            int node = (int)weight.size() - 1;
            parent[na] = node;
            parent[nb] = node;
            if (a > b)
            {
                live.erase(live.begin() + a);
                live.erase(live.begin() + b);
            }
            else
            {
                live.erase(live.begin() + b);
                live.erase(live.begin() + a);
            }
            live.push_back(node);
        }
        int longest = 0;
        for (size_t i = 0; i < used.size(); ++i)
        {
            int d = 0;
            for (int k = (int)i; parent[k] >= 0; k = parent[k])
                ++d;
            len[used[i]] = (unsigned char)d;
            if (d > longest)
                longest = d;
        }
        if (longest <= limit)
            return;
        for (int i = 0; i < n; ++i) // too deep: flatten the frequencies and retry
            if (freq[i])
                freq[i] = (freq[i] + 1) / 2;
    }
}

inline void canonicalCodes(const std::vector<unsigned char> &len, std::vector<unsigned> &code)
{
    int n = (int)len.size();
    code.assign(n, 0);
    unsigned count[16] = {0}, next[16] = {0};
    for (int i = 0; i < n; ++i)
        ++count[len[i]];
    count[0] = 0;
    unsigned c = 0;
    for (int b = 1; b < 16; ++b)
    {
        c = (c + count[b - 1]) << 1;
        next[b] = c;
    }
    for (int i = 0; i < n; ++i)
        if (len[i])
            code[i] = next[len[i]]++;
}

struct Token
{
    unsigned short litLen; // a literal byte, or a match length (dist > 0)
    unsigned short dist;
};

inline int lengthSymbol(int len, int &extraBits, int &extra)
{
    int s = 28;
    while (lenBase[s] > len)
        --s;
    extraBits = lenExtra[s];
    extra = len - lenBase[s];
    return 257 + s;
}
inline int distSymbol(int d, int &extraBits, int &extra)
{
    int s = 29;
    while (distBase[s] > d)
        --s;
    extraBits = distExtra[s];
    extra = d - distBase[s];
    return s;
}

// one dynamic-Huffman block of tokens
inline void writeBlock(BitWriter &w, const std::vector<Token> &toks, bool last)
{
    std::vector<unsigned> lf(286, 0), df(30, 0);
    for (const Token &t : toks)
    {
        int eb, ex;
        if (t.dist == 0)
            ++lf[t.litLen];
        else
        {
            ++lf[lengthSymbol(t.litLen, eb, ex)];
            ++df[distSymbol(t.dist, eb, ex)];
        }
    }
    lf[256] = 1;
    bool anyDist = false;
    for (unsigned f : df)
        if (f)
            anyDist = true;
    if (!anyDist)
        df[0] = 1;
    std::vector<unsigned char> ll, dl;
    huffmanLengths(lf, 15, ll);
    huffmanLengths(df, 15, dl);
    int nlit = 286, ndist = 30;
    while (nlit > 257 && ll[nlit - 1] == 0)
        --nlit;
    while (ndist > 1 && dl[ndist - 1] == 0)
        --ndist;
    // the code-length sequence, run-length coded with 16 / 17 / 18
    std::vector<unsigned char> seq(ll.begin(), ll.begin() + nlit);
    seq.insert(seq.end(), dl.begin(), dl.begin() + ndist);
    std::vector<std::pair<int, int>> rle; // symbol, extra
    for (size_t i = 0; i < seq.size();)
    {
        size_t j = i;
        while (j < seq.size() && seq[j] == seq[i])
            ++j;
        int run = (int)(j - i);
        if (seq[i] == 0)
        {
            while (run >= 11)
            {
                int r = run > 138 ? 138 : run;
                rle.push_back({18, r - 11});
                run -= r;
            }
            if (run >= 3)
            {
                rle.push_back({17, run - 3});
                run = 0;
            }
            while (run-- > 0)
                rle.push_back({0, 0});
        }
        else
        {
            rle.push_back({seq[i], 0});
            --run;
            while (run >= 3)
            {
                int r = run > 6 ? 6 : run;
                rle.push_back({16, r - 3});
                run -= r;
            }
            while (run-- > 0)
                rle.push_back({seq[i], 0});
        }
        i = j;
    }
    std::vector<unsigned> cf(19, 0);
    for (auto &r : rle)
        ++cf[r.first];
    std::vector<unsigned char> cl;
    huffmanLengths(cf, 7, cl);
    int ncode = 19;
    while (ncode > 4 && cl[clOrder[ncode - 1]] == 0)
        --ncode;
    std::vector<unsigned> lc, dc, cc;
    canonicalCodes(ll, lc);
    canonicalCodes(dl, dc);
    canonicalCodes(cl, cc);
    w.put(last ? 1 : 0, 1);
    w.put(2, 2);
    w.put(nlit - 257, 5);
    w.put(ndist - 1, 5);
    w.put(ncode - 4, 4);
    for (int i = 0; i < ncode; ++i)
        w.put(cl[clOrder[i]], 3);
    for (auto &r : rle)
    {
        w.code(cc[r.first], cl[r.first]);
        if (r.first == 16)
            w.put(r.second, 2);
        else if (r.first == 17)
            w.put(r.second, 3);
        else if (r.first == 18)
            w.put(r.second, 7);
    }
    for (const Token &t : toks)
    {
        if (t.dist == 0)
            w.code(lc[t.litLen], ll[t.litLen]);
        else
        {
            int eb, ex;
            int ls = lengthSymbol(t.litLen, eb, ex);
            w.code(lc[ls], ll[ls]);
            if (eb)
                w.put(ex, eb);
            int ds = distSymbol(t.dist, eb, ex);
            w.code(dc[ds], dl[ds]);
            if (eb)
                w.put(ex, eb);
        }
    }
    w.code(lc[256], ll[256]);
}

inline void deflateRaw(const std::string &in, int level, std::string &out)
{
    BitWriter w(out);
    const unsigned char *p = (const unsigned char *)in.data();
    size_t n = in.size();
    if (n == 0 || level == 0)
    {
        // stored blocks
        size_t pos = 0;
        do
        {
            size_t len = n - pos > 65535 ? 65535 : n - pos;
            w.put(pos + len >= n ? 1 : 0, 1);
            w.put(0, 2);
            w.flush();
            out += (char)(len & 255);
            out += (char)(len >> 8);
            out += (char)(~len & 255);
            out += (char)((~len >> 8) & 255);
            out.append(in, pos, len);
            pos += len;
        } while (pos < n);
        return;
    }
    const int window = 32768, hashSize = 1 << 15;
    int chain = level >= 9 ? 1024 : level >= 6 ? 128 : 16;
    std::vector<int> head(hashSize, -1), prev(n, -1);
    auto hashAt = [&](size_t i) { return (int)(((p[i] << 10) ^ (p[i + 1] << 5) ^ p[i + 2]) & (hashSize - 1)); };
    auto insert = [&](size_t i)
    {
        if (i + 2 < n)
        {
            int h = hashAt(i);
            prev[i] = head[h];
            head[h] = (int)i;
        }
    };
    auto longest = [&](size_t i, int &dist)
    {
        int best = 0;
        dist = 0;
        if (i + 2 >= n)
            return 0;
        int maxLen = n - i > 258 ? 258 : (int)(n - i);
        int cand = head[hashAt(i)];
        for (int tries = 0; cand >= 0 && tries < chain; ++tries, cand = prev[cand])
        {
            if ((int)i - cand > window)
                break;
            if (p[cand + best] != p[i + best])
                continue;
            int len = 0;
            while (len < maxLen && p[cand + len] == p[i + len])
                ++len;
            if (len > best)
            {
                best = len;
                dist = (int)i - cand;
                if (len == maxLen)
                    break;
            }
        }
        return best >= 3 ? best : 0;
    };
    std::vector<Token> toks;
    size_t i = 0;
    while (i < n)
    {
        int d1;
        int l1 = longest(i, d1);
        if (l1 && i + 1 < n)
        {
            // lazy: a longer match one byte on wins
            insert(i);
            int d2;
            int l2 = longest(i + 1, d2);
            if (l2 > l1)
            {
                toks.push_back({p[i], 0});
                ++i;
                continue;
            }
            toks.push_back({(unsigned short)l1, (unsigned short)d1});
            for (size_t k = i + 1; k < i + l1; ++k)
                insert(k);
            i += l1;
        }
        else if (l1)
        {
            toks.push_back({(unsigned short)l1, (unsigned short)d1});
            for (size_t k = i; k < i + l1; ++k)
                insert(k);
            i += l1;
        }
        else
        {
            insert(i);
            toks.push_back({p[i], 0});
            ++i;
        }
        if (toks.size() >= 32768)
        {
            writeBlock(w, toks, false);
            toks.clear();
        }
    }
    writeBlock(w, toks, true);
    w.flush();
}

inline void deflateAll(const std::string &in, int wrap, int level, std::string &out)
{
    if (wrap == 2)
    {
        const unsigned char hdr[10] = {0x1F, 0x8B, 8, 0, 0, 0, 0, 0, (unsigned char)(level >= 9 ? 2 : 0), 11};
        out.append((const char *)hdr, 10);
    }
    else if (wrap == 1)
    {
        out += (char)0x78;
        out += (char)(level >= 7 ? 0xDA : 0x9C);
    }
    deflateRaw(in, level, out);
    if (wrap == 2)
    {
        unsigned crc = crc32Update(0, (const unsigned char *)in.data(), in.size());
        unsigned size = (unsigned)in.size();
        for (int k = 0; k < 4; ++k)
            out += (char)((crc >> (8 * k)) & 255);
        for (int k = 0; k < 4; ++k)
            out += (char)((size >> (8 * k)) & 255);
    }
    else if (wrap == 1)
    {
        unsigned ad = adler32Update(1, (const unsigned char *)in.data(), in.size());
        for (int k = 3; k >= 0; --k)
            out += (char)((ad >> (8 * k)) & 255);
    }
}

// hand out what is ready; Z_STREAM_END once all of it is gone
inline int drain(z_stream *s)
{
    __z_state *st = s->state;
    size_t left = st->out.size() - st->given;
    size_t k = left < s->avail_out ? left : s->avail_out;
    for (size_t i = 0; i < k; ++i)
        s->next_out[i] = (Bytef)st->out[st->given + i];
    s->next_out += k;
    s->avail_out -= (uInt)k;
    s->total_out += k;
    st->given += k;
    return st->finished && st->given == st->out.size() ? Z_STREAM_END : Z_OK;
}

inline void take(z_stream *s)
{
    if (s->avail_in && s->next_in)
        s->state->in.append((const char *)s->next_in, s->avail_in);
    s->next_in += s->avail_in;
    s->total_in += s->avail_in;
    s->avail_in = 0;
}

} // namespace __zlib

inline unsigned long crc32(unsigned long crc, const Bytef *buf, uInt len)
{
    return buf ? __zlib::crc32Update((unsigned)crc, buf, len) : 0;
}
inline unsigned long adler32(unsigned long adler, const Bytef *buf, uInt len)
{
    return buf ? __zlib::adler32Update((unsigned)adler, buf, len) : 1;
}

inline int inflateInit2(z_stream *s, int windowBits)
{
    if (!s)
        return Z_STREAM_ERROR;
    s->state = new __z_state();
    s->state->wrap = windowBits < 0 ? 0 : windowBits > 31 ? 3 : windowBits > 15 ? 2 : 1;
    s->msg = nullptr;
    s->total_in = s->total_out = 0;
    return Z_OK;
}
inline int inflateInit(z_stream *s) { return inflateInit2(s, MAX_WBITS); }

inline int inflate(z_stream *s, int flush)
{
    if (!s || !s->state || s->state->deflating)
        return Z_STREAM_ERROR;
    __z_state *st = s->state;
    if (!st->finished)
    {
        size_t had = st->in.size();
        __zlib::take(s);
        if (st->in.size() != had || st->out.empty())
        {
            // decoded again from the start: the part already handed out comes out the same
            st->out.clear();
            const char *msg = nullptr;
            int r = __zlib::inflateAll(st->in, st->wrap, st->out, msg);
            if (r < 0)
            {
                s->msg = msg;
                return Z_DATA_ERROR;
            }
            if (r == 0)
                st->finished = true;
        }
    }
    if (s->avail_out == 0 && st->given < st->out.size())
        return Z_BUF_ERROR;
    size_t before = st->given;
    int r = __zlib::drain(s);
    if (!st->finished && st->given == before)
        return Z_BUF_ERROR; // no progress until more input comes
    return r;
}

inline int inflateEnd(z_stream *s)
{
    if (!s || !s->state)
        return Z_STREAM_ERROR;
    delete s->state;
    s->state = nullptr;
    return Z_OK;
}

inline int deflateInit2(z_stream *s, int level, int method, int windowBits, int memLevel, int strategy)
{
    if (!s || method != Z_DEFLATED)
        return Z_STREAM_ERROR;
    s->state = new __z_state();
    s->state->deflating = true;
    s->state->wrap = windowBits < 0 ? 0 : windowBits > 15 ? 2 : 1;
    s->state->level = level == Z_DEFAULT_COMPRESSION ? 6 : level;
    s->msg = nullptr;
    s->total_in = s->total_out = 0;
    return Z_OK;
}
inline int deflateInit(z_stream *s, int level)
{
    return deflateInit2(s, level, Z_DEFLATED, MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
}

inline int deflate(z_stream *s, int flush)
{
    if (!s || !s->state || !s->state->deflating)
        return Z_STREAM_ERROR;
    __z_state *st = s->state;
    if (!st->finished)
    {
        __zlib::take(s);
        if (flush != Z_FINISH)
            return Z_OK; // compressed in one go at the end
        __zlib::deflateAll(st->in, st->wrap, st->level, st->out);
        st->finished = true;
    }
    if (s->avail_out == 0 && st->given < st->out.size())
        return Z_BUF_ERROR;
    return __zlib::drain(s);
}

inline int deflateEnd(z_stream *s)
{
    if (!s || !s->state)
        return Z_STREAM_ERROR;
    delete s->state;
    s->state = nullptr;
    return Z_OK;
}

inline uLong compressBound(uLong n) { return n + n / 1000 + 64; }
