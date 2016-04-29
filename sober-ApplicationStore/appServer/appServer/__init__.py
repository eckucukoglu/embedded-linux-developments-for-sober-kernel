import OpenSSL
from OpenSSL import crypto as c
import os
chain_cert = open('certificate/ca-chain.cert.pem','rt').read()
chain_certificate = c.load_certificate(c.FILETYPE_PEM,chain_cert)
root_cert = open('certificate/ca.cert.pem','rt').read()
root_certificate = c.load_certificate(c.FILETYPE_PEM,root_cert)
store = c.X509Store()
store.add_cert(root_certificate)
store.add_cert(chain_certificate)
