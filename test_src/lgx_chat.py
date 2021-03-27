#! /usr/bin/python
import socket
import _thread
import json

uid = ''
tid = ''

class Colored(object):
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    BLUE = '\033[34m'
    FUCHSIA = '\033[35m'
    CYAN = '\033[36m'
    WHITE = '\033[37m'
    RESET = '\033[0m'
    def color_str(self, color, s):
        return '{}{}{}'.format(
            getattr(self, color),
            s,
            self.RESET
        )
    def red(self, s):
        return self.color_str('RED', s)
    def green(self, s):
        return self.color_str('GREEN', s)
    def yellow(self, s):
        return self.color_str('YELLOW', s)
    def blue(self, s):
        return self.color_str('BLUE', s)
    def fuchsia(self, s):
        return self.color_str('FUCHSIA', s)
    def cyan(self, s):
        return self.color_str('CYAN', s)
    def white(self, s):
        return self.color_str('WHITE', s)
color = Colored()


sk = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sk.connect(('127.0.0.1', 80))



def post(args, d):
    p =  b'POST /' + b'?' + args + b' HTTP/1.1\r\n'
    p += b'Content-Length: ' + str(len(d)).encode() + b'\r\n'
    p += b'Connection: Keep-Alive\r\n'
    p += b'\r\n'
    p += d
    sk.send(p)

def get(args):
    p =  b'GET /' + b'?' + args + b' HTTP/1.1\r\n'
    p += b'Connection: Keep-Alive\r\n'
    p += b'\r\n'
    sk.send(p)

def get_recv():
    s = sk.recv(1024);
    if(len(s) <= 10):
        return
    sa = s.split(b'\r\n\r\n')
    header = sa[0]
    length = 0
    sl = header.split(b"Content-Length: ")
    if(len(sl) >= 2):
        length = int(sl[1].split(b"\r\n")[0])
    body = sa[1]
    while True:
        if(len(body) >= length):
            break
        body += sk.recv(1)
    return body


def recv_thread():
    while True:
        recv = json.loads(get_recv())
        request = recv['request']
        if(request == 'recv_user_msg'):
            msg = recv['content']['msg']
            uid = recv['content']['uid']
            print("recv msg: " + color.green(msg))
        elif(request == 'get_all_user_info'):
            show_all_user(recv['content'])
        #print(color.yellow(recv))
def login():
    global uid
    # login
    name = input('Input your name:')
    args = 'request=login'
    send = {'content': {"name": name}}
    data = json.dumps(send);
    post(args.encode(), data.encode())
    s = get_recv()
    #print(s)
    recv = json.loads(s);
    if(recv['code'] != 0):
        print(color.red('login failed!'))
        return False
    print(color.green('login ok!'))
    uid = recv['content']['uid']
    print('your uid: ' + color.red(uid))
    return True

def group_create(name):
    args = 'request=create_group'
    send = {'content': { "name" : name, "uid": uid}}
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


def send_user_chat(tid, msg):
    args = 'request=send_msg_to_user'
    send = {'content': { "uid" : uid, "tid": tid, "msg" : msg}}
    data = json.dumps(send);
    #print('send:\n' + data)
    post(args.encode(), data.encode())

def clear_cmd():
   print(color.green('\x1b[H\x1b[2J'))
def munu():
    clear_cmd()
    print(color.blue('\t lgx simple chat client\n'))
    print(color.blue('\t 1. Chat with user\n'))
    print(color.blue('\t 2. Chat with group\n'))

def get_all_user_info():
    # login
    args = 'request=get_all_user_info'
    get(args.encode())
    s = get_recv()
    #print(s)
    recv = json.loads(s);
    if(recv['code'] != 0):
        print(color.red('get_all_user_info failed!'))
        return False

    show_all_user(recv['content'])
    return True

users = {}
def select():
    s = input()
    if(s == '1'):
        login()
        get_all_user_info()
        chat_with_user()
    elif(s == '2'):
        login()

def show_all_user(ua):
    global users
    i = 1
    for c in ua:
        print( color.green(' *' + '%02d' % i + ' name: ' + c['name'] + " network: " + c['network']))
        users[c['name']] = [c['uid'], c['network']]
        i += 1

def chat_with_user():
    _thread.start_new_thread(recv_thread, ())
    while True:
        name = input(color.yellow("input name to chat: "))
        uid = users[name][0]
        if(uid != None):
            break
        print('No this user!')
    print('chating uid ' + uid)
    while True:
        s_msg = input()
        send_user_chat(uid , s_msg)

munu()
select()




