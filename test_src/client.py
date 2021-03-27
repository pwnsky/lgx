#! /usr/bin/python
import socket
import _thread
import json

print('lgx simple chat\n')

sk = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sk.connect(('127.0.0.1', 80))

def recv_thread():
    while True:
        recv = json.loads(get_recv())
        request = recv['request']
        if(request == 'recv_user_msg'):
            msg = recv['content']['msg']
            uid = recv['content']['uid']
            #print("recv user msg" + msg)
        print(recv)

def post(args, d):
    p =  b'POST /' + b'?' + args + b' HTTP/1.1\r\n'
    p += b'Content-Length: ' + str(len(d)).encode() + b'\r\n'
    p += b'Connection: Keep-Alive\r\n'
    p += b'\r\n'
    p += d
    sk.send(p)

def get_recv():
    s = sk.recv(1024);
    if(len(s) <= 10):
        return
    s = s.decode()
    sa = s.split('\r\n\r\n')
    header = sa[0]
    body = sa[1]
    return body

uid = ''
def login():
    global uid
    # login
    name = input('Input your name:')
    args = 'request=login'
    send = {'content': {"name": name}}
    data = json.dumps(send);
    post(args.encode(), data.encode())
    print("logining ...")
    s = get_recv()
    #print(s)
    recv = json.loads(s);
    if(recv['code'] != 0):
        print("Failed!")
        return False
    print("logined!")
    uid = recv['content']['uid']
    print('your uid:' + uid)
    return True

login()


def group_create():
    args = 'request=create_group'
    send = {'content': {"name": name}}
    data = json.dumps(send);
    post(args.encode(), data.encode())

def group_chat():
    a = 0

def join_group():
    gid = 'none'
    args = 'request=join_group'
    send = {'content': { {"uid": uid}, {"gid", gid} }}
    data = json.dumps(send)
    post(args.encode(), data.encode())


def user_chat(tid, msg):
    args = 'request=send_msg_to_user'
    send = {'content': { "uid" : uid, "tid": tid, "msg" : msg}}
    data = json.dumps(send);
    #print('send:\n' + data)
    post(args.encode(), data.encode())

_thread.start_new_thread(recv_thread, ())
while True:
    s_msg = input()
    user_chat('6353e37385242207823b20dbfa712e8a', s_msg)
