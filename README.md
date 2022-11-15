# FSSN_ZMQ_C

## 풀스택 주제
 * ### [ZMQ](https://zeromq.org/)

## 사용 언어
* ### C

최근에 wsl2를 이용하여 linux에서 C를 공부하고 있었기 때문에, 이를 활용하여 ZMQ를 구현한다면 실력 향상에 도움이 될 것이라고 판단하였다.

## 본문
  * C의 라이브러리인 : czmq를 이용하여 구현하였다.

  ### 설치방법
  * https://zeromq.org/languages/c/#czmq 를 참고하면 된다. 혹은 아래의 코드를 복사하면 된다.
``` 
    apt-get install libczmq-dev
```
  ### 컴파일방법
  gcc를 통해 컴파일 할 때 뒤에 -l 옵션에 czmq와 zmq의 헤더를 포함하는 명령어를 지정해주어야한다.

  ```
    gcc -o example.o example.c -lczmq -lzmq
  ```

  ### 구현 파일
    * [REQ-REP](https://github.com/rlaminseok0824/FSSN_ZMQ_C/tree/main/REQ-REP)
      * lec-05-prg-01-req-rep-basic-client.c
      * lec-05-prg-01-req-rep-basic-server.c
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

 


