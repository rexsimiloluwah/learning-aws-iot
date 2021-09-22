#ifndef secrets_h
#define secrets_h


// Define the AWS IoT endpoint 
const char* aws_iot_endpoint = "<your-endpoint>.amazonaws.com";

// AWS IoT Core Root CA
// PROGMEM is used to place the string in Flash memory and load it in RAM when needed
static char* aws_cert_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate 
static char* aws_device_cert PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUfaE4DtZCp816YJjuPFwLP7YXnwAwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIxMDkwNjIzMzgx
MFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANl4x/8mCFhlw0r0FmVV
lqMhl/DyIslUcHABI9+cRxnDqMC32s97QuMa0OcWdLdrEWWeI4GvoXZRH0y/vkrZ
yyVQaadYidM1pwrWc4F627dtm1DYIQv5t7UUCnO1UcmAViZ7KyYBbYLSb5TN2FNM
iHnYOYSD86DlFbUroUcw2X4mMIPi6qy/8c57Gnn+9xC7CjVkh/gjf2sdWiCeGhPT
qYi73SoWYzmDMIWkBAbBo8cxLd5wjzNJw2OCNhsVDnWIhYKNWhX2Er9HlAeReZz9
fL8xQXWAUhDtHCjFuv07G2DmwaLcsim6LsmduNw05JZMyRMvSJi/+7JuP4ZwimPX
fSsCAwEAAaNgMF4wHwYDVR0jBBgwFoAUXJGoQNhliQgdWMDeY9A58NBgekMwHQYD
VR0OBBYEFKjuCPbUViHKOQJpulCUl5lRTCVAMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAIKtMU20/K2FHzxAzyLkzuuh84
6sq8L6+DP5pHJlYqyWYnebd75CCc96jh0587hditOOpLGiXHyUhjAlS3VuvCvJSE
QszUxPye9d/o9NsyQSyrZKO2u30P52LcNP5PD+pspgMeZWtN4IP2LzmAlYvqhqd4
qhBZt/1UOG77cUJq07+57jOVtSd9tJfRdsSzA+x1iDqmHl/F0PDEOmBXXfgeXYXW
n0etnwM9FXegHBVBJEmA7gDgSO5XCPwtPrWUUkOCogXT1qY6DVUj5ryBJuVRYgoK
0Q3aZzWfpfMbQ80QyXwSKbyEmKmGpcQl8x+Ym3sfpJUoYxbrvAt8SnT1EW5c
-----END CERTIFICATE-----
)KEY";

// Device Private Key 
static char* aws_cert_private_key PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEA2XjH/yYIWGXDSvQWZVWWoyGX8PIiyVRwcAEj35xHGcOowLfa
z3tC4xrQ5xZ0t2sRZZ4jga+hdlEfTL++StnLJVBpp1iJ0zWnCtZzgXrbt22bUNgh
C/m3tRQKc7VRyYBWJnsrJgFtgtJvlM3YU0yIedg5hIPzoOUVtSuhRzDZfiYwg+Lq
rL/xznsaef73ELsKNWSH+CN/ax1aIJ4aE9OpiLvdKhZjOYMwhaQEBsGjxzEt3nCP
M0nDY4I2GxUOdYiFgo1aFfYSv0eUB5F5nP18vzFBdYBSEO0cKMW6/TsbYObBotyy
KbouyZ243DTklkzJEy9ImL/7sm4/hnCKY9d9KwIDAQABAoIBAAgyOydsoYfjXfbn
OCjdju3bCET2e0UnP42ZHWcCFtPrweJxi3eiZ6Tzj8gF2qRHDsLLsP7kRauV5x4X
Y+6iQrvHShQajCc/sGoCJxxMKxfAs8faPXScFsZmWPZ/cPOtOJH+tmOaTixODhKH
p42MJuCgu5o49uereqP3Xv614pyUdhkdqnIqP2dnQGpNX5ksXRWzH2lo0DUkgTe+
NxpbicSY5STanb1xGNWR7JH49SfL6pE1as4B/WUCKYYoy/NNGomZtTPvDwjXbnd+
KgP8Ih1hUQJgu+WG/NzXpFSUWd9e21ly0ernCyKP5pjEAMT1wKOAY0SGnYgFwQEQ
31WHzOECgYEA8eLvV6BNF0nBP1lIVX+vqx9TWVv6PSLyHYIFjP7vG20m278k61hT
Vm1/3PgteaIu2Z8+SW4Ur9tG5uwpsYkmOp+wGKaF8Pu3AS4ZXsWBtf9IokEZI2PD
rD39BAxxAAJb1UJWXCY9iJuZWmEElh0yi/WH3SqrBc5/taZFhcq1OSkCgYEA5ikp
7duqCWbPBKGrCIgij5W8FDq2dhu8TPgz7dNS0ate7iqAb1/1yGfeSDvQ/X/lr4tF
wZSepcYMnUwdGcCJfI/sgvmr8AZ4ZIdRZJ7g7BygB+qipTe+2dvOmrn+tgjbe8nP
KNqNOm5gSNqvoRS4pd+RkyJvH+dSjVCH114lijMCgYA26YvoWq3loYrLdNtk/Y3F
zWDAoo7Wnj5U0JWuApoJoef2bxhUjUKuFkG2ymiYemckZHBhfyoXGjKobeixMy+I
k4QItJUnth0/mVWNHE1sIb49h4XLcPSU/6WW67QjimWKrnqohM3nW1ufFvNRmONG
vsDPRsZ4lP6NIGgcDX7CsQKBgFN5kHx1E9lvERwT6m0gurAq1Vjc5no1mvireGIE
J+Blb/ufLI+4VPMHQOYf069tKIefUoBf/54OE1slVNSXMQYdSOFD3ikin52OOmXs
/ZSE2UGlxrF3Y1+uSeNyG8A3vCrMSbb+dQRBLeieb95THpkCWdDsRzNh+V3JXEsv
GRxjAoGBAOlX8i51hfBuQPhlSz/9/q/THh7U6IvvlIWIM+VLHb0AJjcqz1Z8WpUT
qnh+KZPrLtgmEY8sgmVVY5FVYU7Q8MSbSQNduHkHGrSp471ERPdmYnjdQnWTW66Z
W1u6gkYM1h1Yr7eWMeVf1JAMk/SsGjg2eXlGaJlxqv02odGn4dCX
-----END RSA PRIVATE KEY-----
)KEY";

#endif
