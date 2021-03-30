package main
import (
    "fmt"
    "log"
    "net"
    "time"
    "strings"
    "os/exec"
    "bytes"
    "strconv"
)

type LgxSock struct {
    host string 
    io net.Conn
    body []byte
    header map[string]string
    //type callback func(l *LgxSock) Recv()
}

func (l *LgxSock) Init(host string) {
    l.host = host;
    l.header = make(map[string]string) //初始化map
    l.body = make([]byte, 0x1000)
}

func (l *LgxSock) Run() {
    conn, err := net.Dial("tcp", l.host)
    if err != nil {
        log.Println("dial erro:", err)
        return
    }
    defer conn.Close()
    l.io = conn
    fmt.Println("connected!")
    l.Get("/", "?a=1")
    l.GoRecv()
}

func (l *LgxSock) Get(path string, args string) {
    buf :=  "GET " + path + "?" + args + " HTTP/1.1\r\n"
    buf += "Connection: Keep-Alive\r\n"
    buf += "\r\n"
    l.io.Write([]byte(buf))
}

func (l *LgxSock) GoRecv() {
    time.Sleep(time.Millisecond * 100)

    for true {
        header_length := 0
        recv_buf := make([]byte, 4096)
        recv_len, err := l.io.Read(recv_buf)
        if err != nil {
            if(recv_len == 0) {
                fmt.Printf("Host disconnected! err:%v\n", err)
            }
            return
        }
        recv_buf_split := strings.Split(string(recv_buf), "\r\n\r\n")
        if len(recv_buf_split) <= 0 {
            log.Println("parse http err")
            return
        }

        header_data := recv_buf_split[0]
        header_length = len(header_data) + 4
        header_info := strings.Split(header_data, "\r\n")
        for i, k := range header_info {
            if i == 0 {
                continue
            }
            //fmt.Printf("* %d : %s\n", i, k)
            ta := strings.Split(k, ": ")
            //fmt.Printf("key: %s value: %s\n", ta[0], ta[1])
            if len(ta) < 2 {
                log.Println("parse http header value err")
                return
            }
            l.header[ta[0]] = ta[1]
        }
        val, ok := l.header [ "Content-Length" ]
        body_length := 0
        if ok {
            length, err := strconv.Atoi(val)
            if err != nil {
                length = 0
            }
            body_length = length 
        }
        fmt.Printf("length: %d\n", body_length)

        body_recved_len := recv_len - header_length
        fmt.Printf("body_recved_len: %d\n", body_recved_len)
        //fmt.Printf("body: %s\n", recv_buf)

        for i:= 0; i <= body_recved_len; i++ {
            l.body = append(l.body, recv_buf[header_length + i])
        }
        fmt.Printf("body: %s\n", l.body)
        body_len_left := body_length - body_recved_len
        // left
        for true {
            recv_len, err := l.io.Read(recv_buf)
            if err != nil {
                if(recv_len == 0) {
                    fmt.Printf("Host disconnected! err:%v\n", err)
                }
                return
            }
            if(body_len_left <= 0) {
                break
            }
            body_len_left -= recv_len
            for i:= 0; i <= recv_len; i++ {
                l.body = append(l.body, recv_buf[i])
            }
        }
    }

    //fmt.Printf("header_data:\n%s\n", string(header_data))
}

func main() {
    fmt.Println("lgx chat client for golang")
    lh := LgxSock{}
    lh.Init("127.0.0.1:80")
    lh.Run()
    time.Sleep(time.Second * 1)
    var name string
    fmt.Println("Your name:")
    fmt.Scanln(&name)
    cmd := exec.Command("whoami")
    var out bytes.Buffer
    cmd.Stdout = &out
    err := cmd.Run()
    if err != nil {
        log.Fatal(err)
    }
    fmt.Printf("cmd result:%s\n", out.String())
}
