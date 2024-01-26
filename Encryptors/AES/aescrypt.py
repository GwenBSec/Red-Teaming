import sys 
from Crypto.Cipher import AES
from os import urandom 
import hashlib

KEY = urandom(16)

def pad(s):

    k = hashlib.sha256(key).digest()
    iv = 16 * '\x00'
    plaintext = pad(plaintext)
    cipher = AES.new(k, AES.MODE_CBC, iv)

    return cipher.encrypt(bytes(plaintext))

try:
    plaintext = open(sys.argv[1], "rb").read()
except:
    print("File argument needed!" %s <raw paylaod file>" % sys.argv[0])
    sys.exit()

ciphertext = aesenc(plaintext, KEY)
print('AESkey[] = { 0x' + ', 0x'.join(hex(ord(x))[2:] for x in KEY) + ' };')
print('payload[] = { 0x' + ', 0x'.join(hex(ord(x))[2:] for x in ciphertext) + ' };')
