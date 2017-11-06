/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 */

#ifndef __QCOM__SECURITY_H__
#define __QCOM__SECURITY_H__

#ifdef LIBCUST_INDIRECT_ACCESS
typedef struct
{
	char *
	(* _MDString)(char *string);
	char *
	(* _secur_md5_string)(char *input);
	void
	(* _qcom_sec_md5_init)();
	void
	(* _qcom_sec_md5_update)(unsigned char *data, int len);
	void
	(* _qcom_sec_md5_final)(char md5_val[]);


}QCOM_SECURITY_API_INDIRECTION_TABLE;


extern QCOM_SECURITY_API_INDIRECTION_TABLE qcomSecurityApiIndirectionTable;
#define QCOM_SECURITY_API_FN(fn) qcomSecurityApiIndirectionTable.fn

#else


char *
_MDString(char *string);
char *
_secur_md5_string(char *input);
void
_qcom_sec_md5_init();
void
_qcom_sec_md5_update(unsigned char *data, int len);
void
_qcom_sec_md5_final(char md5_val[]);


#define QCOM_SECURITY_API_FN(fn) fn


#endif
#define AES_256_SUPPORT 1
#ifdef AES_256_SUPPORT

typedef enum
{
    QCOM_AES_CTX_DECRYPT,
    QCOM_AES_CTX_ENCRYPT
}Qcom_AES_Ctx_Type;

#define QCOM_AES_128_LEN    16
#define QCOM_AES_256_LEN    32
#endif
#define RSA_CRYPTO_SUPPORT  1
#define RSA_SIGN_DO_ENABLE 1
void *_qcom_aes_encrypt_init(const unsigned char *key, int len);
void _qcom_aes_encrypt(void *ctx, unsigned char *plain, unsigned char *crypt);
void _qcom_aes_encrypt_deinit(void *ctx);
void *_qcom_aes_decrypt_init(const unsigned char *key, int len);
void _qcom_aes_decrypt(void *ctx, unsigned char *crypt, unsigned char *plain);
void _qcom_aes_decrypt_deinit(void *ctx);
int _qcom_rsa_public_encrypt(unsigned char * in ,unsigned int inlen,const const char *pub_key,unsigned char *out, unsigned char padding_mode) ;
int _qcom_rsa_public_decrypt(unsigned char * in ,unsigned int len,const char *pub_key,unsigned char *out,unsigned char padding_mode);
int _qcom_rsa_private_decrypt(unsigned char * in ,unsigned int len,const char *pri_key,unsigned char *out,unsigned char padding_mode);
int _qcom_rsa_sign_init(const unsigned char * cert);
int _qcom_rsa_sign_verify(unsigned char * sign_data,unsigned int sign_data_len);
#ifdef RSA_SIGN_DO_ENABLE
int _qcom_rsa_sign_do(unsigned char * pri_key,unsigned char * sign_data,unsigned int * sign_data_len);
#endif
void _qcom_rsa_sign_deinit();

#define MDString(str) \
	QCOM_SECURITY_API_FN(_MDString(str))
#define secur_md5_string(str) \
	QCOM_SECURITY_API_FN(_secur_md5_string(str))
#define qcom_sec_md5_init() \
	QCOM_SECURITY_API_FN(_qcom_sec_md5_init())
#define qcom_sec_md5_update(str, len) \
	QCOM_SECURITY_API_FN(_qcom_sec_md5_update((str), (len)))
#define qcom_sec_md5_final(str) \
	QCOM_SECURITY_API_FN(_qcom_sec_md5_final(str))

#define qcom_aes_encrypt_init(key, len) \
		_qcom_aes_encrypt_init((key), (len))
		
#define qcom_aes_encrypt(ctx, plain, crypt) \
		_qcom_aes_encrypt((ctx), (plain), (crypt))
		
#define qcom_aes_encrypt_deinit(ctx) \
		_qcom_aes_encrypt_deinit((ctx))
	
#define qcom_aes_decrypt_init(key, len) \
			_qcom_aes_decrypt_init((key), (len))
			
#define qcom_aes_decrypt(ctx, plain, crypt) \
			_qcom_aes_decrypt((ctx), (plain), (crypt))
			
#define qcom_aes_decrypt_deinit(ctx) \
			_qcom_aes_decrypt_deinit((ctx))

#ifdef RSA_CRYPTO_SUPPORT

#define  QCOM_RSA_PKCS1_PADDING  1
#define  QCOM_RSA_NO_PADDING     0
#define qcom_rsa_public_encrypt(in,in_len,pub_key,out,padding_mode)\
            _qcom_rsa_public_encrypt((in),(in_len),(pub_key),(out),(padding_mode))

#define qcom_rsa_public_decrypt(in,in_len,pub_key,pub_key_len,out,outlen)         \
            _qcom_rsa_public_decrypt((in),(in_len),(pub_key),(pub_key_len),(out),(outlen))

#define qcom_rsa_private_decrypt(in,in_len,pri_key,out,outlen)        \
            _qcom_rsa_private_decrypt((in),(in_len),(pri_key),(out),(outlen))

#define qcom_rsa_sign_init(c) \
        _qcom_rsa_sign_init((c))

#define qcom_rsa_sign_deinit() \
      _qcom_rsa_sign_deinit()

#define qcom_rsa_sign_verify(d,l) \
    _qcom_rsa_sign_verify((d),(l))

#ifdef RSA_SIGN_DO_ENABLE
#define qcom_rsa_sign_do(k,d,l) \
    _qcom_rsa_sign_do((k),(d),(l))
#endif

#endif


#endif
