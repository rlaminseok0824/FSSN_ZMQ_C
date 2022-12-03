# FSSN_ZMQ_C

## 풀스택 주제
 * ### [ZMQ](https://zeromq.org/)

## 운영체제
 * ### Ubuntu(Linux)

## 사용 언어
* ### C

최근에 wsl2를 이용하여 linux에서 C를 공부하고 있었기 때문에, 이를 활용하여 ZMQ를 구현한다면 실력 향상에 도움이 될 것이라고 판단하였다.

## 본문
  * C의 라이브러리인 : czmq를 이용하여 구현하였다.

  ### 설치방법
  * https://zeromq.org/languages/c/#czmq 를 참고하면 된다. 혹은 아래의 코드를 복사하면 된다.
``` shell
    $ apt-get install libczmq-dev
```
  ### 컴파일방법
  gcc를 통해 컴파일 할 때 뒤에 -l 옵션에 czmq와 zmq의 헤더를 포함하는 명령어를 지정해주어야한다.

  ``` shell
    $ gcc -o example.o example.c -lczmq -lzmq
  ```
 

  ### 실행 방법
  * 각 구현 패턴들마다 폴더를 나누어 저장하였다.(밑에 구현 파일을 통해 확인 가능하다.)
  * 각 폴더들에는 그것을 구현한 소스코드와 컴파일한 파일들이 있다.
    * 일반적으로 서버 파일은 server 로 클라이언트 파일은 client로 파일이 있다.
  * 직접 실행해보기
    * 서버 프로그램 먼저 실행시키기. 다음 명령어를 쳐서 서버를 먼저 실행시킨다.
      ``` shell
        $ ./server
      ``` 
    * 다음 다른 명령어창을 띄어 다음 명령어를 쳐서 클라이언트 프로그램을 실행시킨다.
      ```
        $ ./client
      ```
    * 일부 폴더의 예제 같은 경우 추가적인 argv 즉 입력값을 가질 수도 있다. 따라서 다음과 같이 추가적인 입력값을 넣어야한다.
      ```
       # PUSH-PULL-v2/
       $ ./client client#1
      ```
  ### 구현 파일
  * [REQ-REP](https://github.com/rlaminseok0824/FSSN_ZMQ_C/tree/main/REQ-REP)
    * lec-05-prg-01-req-rep-basic-client.c
    * lec-05-prg-02-req-rep-basic-server.c
  * [PUB-SUB](https://github.com/rlaminseok0824/FSSN_ZMQ_C/tree/main/PUB-SUB)
    * lec-05-prg-03-pub-sub-basic-server.c
    * lec-05-prg-04-pub-sub-basic-client.c
  * [PUSH-PULL](https://github.com/rlaminseok0824/FSSN_ZMQ_C/tree/main/PUSH-PULL)
    * lec-05-prg-05-pub-sub-and-pull-push-server.c
    * lec-05-prg-06-pub-sub-and-pull-push-client.c
  * [PUSH-PULL-v2](https://github.com/rlaminseok0824/FSSN_ZMQ_C/tree/main/PUSH-PULL-v2)
    * lec-05-prg-07-pub-sub-and-pull-push-server-v2.c
    * lec-05-prg-08-pub-sub-and-pull-push-client-v2.c
  * [DEALER-ROUTER](https://github.com/rlaminseok0824/FSSN_ZMQ_C/tree/main/DEALER-ROUTER)
    * lec-05-prg-09-dealer-router-async-server.c
    * lec-05-prg-10-dealer-router-async-client.c
    * lec-05-prg-11-dealer-router-async-client-thread.c
  * [P2P-DECHAT](https://github.com/rlaminseok0824/FSSN_ZMQ_C/tree/main/P2P-DECHAT)
    * lec-05-prg-12-p2p-dechat.c

  ### 시연 영상
  * REQ-REP.mp4
  * PUB_SUB_v1,_v2.mp4 (기존 코드대로 작성한 버전은 v1. 하지만 너무 빠른 PUB으로 인해 영상 촬영의 어려움이 생겨 sleep을 넣어 v2 촬영)
  * PUSH_PULL.mp4
  * PUSH_PULL_v2.mp4
  * DEALER_ROUTER_four_thread.mp4
  * DEALER_ROUTER_single_thread.mp4
  * P2P_DECHAT.mp4



