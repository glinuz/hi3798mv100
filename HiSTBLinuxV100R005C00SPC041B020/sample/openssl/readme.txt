Summary:
   This sample provide one reference on how to use OpenSSL.    
   The sample use bi-directional certificate authentication between client and server side.

How to use this sample:

1、First compile sample:
  After compile, the two executable program:openss_client_test and openss_server_test will be generated.
 
2、You need generate the following certificate and put your key files below into the directory key. 
ca.crt       (Root CA certificate)
client.crt   (Client certificate)
client.key   (Client private key)
server.crt   (Server certificate)
server.key   (Server private key)

3、Please first run server program as below:
./openssl_server_test

4、Then run Client program as below:
./openssl_client_test 10.67.217.19

The 10.67.217.19 is ip address of the server.

You can input data in Client and press ENTER key to start new communication. 


[Note]:
1. You must make sure the system timer on your board accord with your certificate.
For example: The Validity of your certificate is like the following: 
Validity
            Not Before: May  4 07:02:20 2016 GMT
            Not After : May  3 07:02:20 2021 GMT  
You should adjust your system timer on your board as below. Otherwise, the certificate verification will fail. 
date -s "2016-05-06 12:00:00"

2.If your private key is encrypted, please ensure your password must match the value of CLIENT_PRIVATE_KEY_PASSWD or SERVER_PRIVATE_KEY_PASSWD, which are defined in corresponding sample code.