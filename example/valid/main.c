#include <stdio.h>
#include <he/err.h>
#include <he/x.h>

int main() {
    int v, v0, e, e0, t, t0;
    int h, h0, f, cnt;
    ini("/dev/stdin");

    for (v = 0; v < NV; v++) { /* from vert and back */
        v0 = ver(hdg_ver(v));
        if (v != v0) ER("v=%d != v0=%d", v, v0);
    }

    for (e = 0; e < NE; e++) { /* from edg and back */
        h = hdg_edg(e);
        e0 = edg(h);
        if (e != e0) ER("e=%d h=%d e0=%d", e, h, e0);
    }

    for (t = 0; t < NT; t++) { /* from tri and back */
        h = hdg_tri(t);
        t0 = tri(h);
        if (t != t0) ER("%d != %d", t, t0);
    }

    for (v = 0; v < NV; v++) { /* walk over tri */
        cnt = 0;
        h0 = h = hdg_ver(v);
        do {
            cnt++;
            h = nxt(h);
        } while (h != h0);
        if (cnt != 3) ER("%d != 3 for h=%d", cnt, h0);
    }

    for (h = 0; h < NH; h++) { /* flip-flip */
        if (bnd(h)) continue;
        f = flp(h);
        h0 = flp(f);
        if (h != h0) ER("%d != %d", h, h0);
    }
    fin();
    MSG("ok");
    return(HE_OK);
}
