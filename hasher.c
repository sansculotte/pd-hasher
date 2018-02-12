// vim: softtabstop=4 :shiftwidth=4 :expandtab
/*****************************************************************
 *
 * Hashing strings
 *
 * u@sansculotte.net 2018
 *
**/
#include <string.h>
#include <stdlib.h>
#include <gcrypt.h>
#include "m_pd.h"

#define VERSION "v0.0.1"

#define TRUE 1
#define FALSE 0


typedef int t_algorithm;

static t_class *hasher_class;

typedef struct _hasher {
    t_object  x_obj;
    t_outlet  *s_out;
    t_algorithm algorithm; 
} t_hasher;

t_algorithm select_algorithm(t_symbol *algorithm);
char* algorithm_name(t_algorithm algorithm);


/**
 * convert digest raw bytes into hexpairs
 */
void to_hex(size_t length, unsigned char* digest, char* hexdigest)
{
    size_t i;
    for(i=0; i < length; i++) {
        sprintf(hexdigest + (2 * i), "%02x", *(digest + i));
    }
}

/**
 * Select algorithm from user input
 * default to md5
 */
t_algorithm select_algorithm(t_symbol *algorithm) {

    char* a = algorithm->s_name;

    if (strcmp(a, "sha-256") == 0 || strcmp(a, "sha256") == 0) {
        return GCRY_MD_SHA256;
    }
    if (strcmp(a, "sha-1") == 0 || strcmp(a, "sha1") == 0) {
        return GCRY_MD_SHA1;
    }
    if (strcmp(a, "tiger") == 0 || strcmp(a, "tiger") == 0) {
        return GCRY_MD_TIGER;
    }
    return GCRY_MD_MD5;
}

/**
 * constructor
 */
void* hasher_new(t_symbol* s)
{
    t_hasher *x = (t_hasher *)pd_new(hasher_class);

    x->algorithm = select_algorithm(s);
    x->s_out = outlet_new(&x->x_obj, &s_symbol);
    
    return (void *)x;
}

/**
 * info and pointers
 */
void _hasher_about(t_hasher *x)
{
    char v[64];
    sprintf(v, "[hasher] version: %s\nalgorithm: %s", VERSION, gcry_md_algo_name(x->algorithm));
    post(v);
    post("https://github.com/sansculotte/pd-hasher");
}

/**
 * change the hashering algorithm
 */
void _hasher_algorithm(t_hasher *x, t_symbol *s) {
    x->algorithm = select_algorithm(s);
}

/**
 * hash a string
 */
void _hasher_hexdigest(t_hasher *x, t_symbol *s, int argc, t_atom* argv)
{
    int length;
    size_t digsize = gcry_md_get_algo_dlen(x->algorithm);
    unsigned char* digest = malloc(digsize);
    char * hexdigest = calloc(digsize * 2 + 1, sizeof(char));
    char *buf;
    t_binbuf *b = 0;

    b = binbuf_new();
    binbuf_add(b, argc, argv);
    binbuf_gettext(b, &buf, &length);
    
    gcry_md_hash_buffer(x->algorithm, digest, buf, length);
    to_hex(digsize, digest, hexdigest);
    outlet_symbol(x->s_out, gensym(hexdigest));

    free(digest);
    free(hexdigest);
}

/**
 * create object
 */
void hasher_setup(void)
{
    hasher_class = class_new(
        gensym("hasher"),
        (t_newmethod)hasher_new,
        0,
        sizeof(t_hasher),
        CLASS_DEFAULT,
        A_DEFSYMBOL,
        0
    );
    class_addmethod(hasher_class, (t_method)_hasher_about, gensym("about"), 0);
    class_addmethod(hasher_class, (t_method)_hasher_algorithm, gensym("algorithm"), A_SYMBOL, 0);
    class_addmethod(hasher_class, (t_method)_hasher_hexdigest, gensym("hex"), A_GIMME, 0);
    class_addmethod(hasher_class, (t_method)_hasher_hexdigest, gensym("hexdigest"), A_GIMME, 0);
}
