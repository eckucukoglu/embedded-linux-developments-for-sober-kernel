import requests
#from secretsharing import PlaintextToHexSecretSharer
#client_serial = "4214BBBK0963"
#shares = PlaintextToHexSecretSharer.split_secret("merhaba",2,3)
#
#r=requests.post("http://localhost:8080/server/",data={'share':shares[1],'serial':client_serial})
#r=requests.get("http://localhost:8080/server/",params={'command':'get','serial':client_serial})
#r=requests.get("http://localhost:8080/server/",params={'serial':client_serial,'command':'get','share':shares[0]})
r=requests.get("https://10.155.10.206:443/application/19",verify='/home/burakmert/Projects/MMIS/ApplicationServer/appServer/certificate/ca-chain.cert.pem')
print(r.text)
#print r.url