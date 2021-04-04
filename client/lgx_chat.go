// lgx chat client source code
// Author: i0gan
// Date  : 2021-03-28
// Github: https://github.com/i0gan/lgx

package main
import (
    "fmt"
    "log"
    "net"
    "time"
    "strings"
//    "os/exec"
    "bytes"
    "strconv"
    "os"
    "encoding/json"
    "os/signal"
    "syscall"
    "github.com/tidwall/gjson"
)

type LoginInfo struct {
    Name  string `json:"name"`
}

type RequestJson struct {
    Request  string `json:"request"`
    Content  interface{} `json:"content"`
}

type LgxSock struct {
    host string 
    io net.Conn
    header map[string]string
    read_chan chan []byte // handle read block
    //type callback func(l *LgxSock) Recv()
}

func (l *LgxSock) Init(host string) {
    l.host = host;
    l.header = make(map[string]string) //初始化map
    l.read_chan = make(chan []byte, 1024) //初始化recv channel
}

func (l *LgxSock) Connect() {
    conn, err := net.Dial("tcp", l.host)
    if err != nil {
        log.Println("dial erro:", err)
        os.Exit(-1)
        return
    }
    defer conn.Close()
    l.io = conn
    fmt.Println("connected!")
    l.GoRecv()
}

func (l *LgxSock) Get(path string, args string) {
    buf :=  "GET " + path + "?" + args + " HTTP/1.1\r\n"
    buf += "Connection: Keep-Alive\r\n"
    buf += "\r\n"
    l.io.Write([]byte(buf))
}

func (l *LgxSock) Post(path string, args string, data []byte) {
    buf :=  "POST " + path + "?" + args + " HTTP/1.1\r\n"
    buf += "Connection: Keep-Alive\r\n"
    buf += "Content-Length: " + strconv.Itoa(len(data)) + "\r\n"
    buf += "\r\n"
    l.io.Write([]byte(buf))
    l.io.Write(data)
}

func (l *LgxSock) GoRecv() {
    for true {
        header_length := 0
        recv_buf := make([]byte,  0x1000)
        body_data := make([]byte, 0x1000) // Renew memory every recv
        fmt.Println("tcp_reading...")
        recv_len, err := l.io.Read(recv_buf)
        fmt.Printf("tcp_readed %d\n", recv_len)
        if err != nil {
            if(recv_len == 0) {
                fmt.Printf("Host disconnected! err:%v\n", err)
            }
            return
        }
        recv_buf_split := bytes.Split(recv_buf, []byte("\r\n\r\n"))
        if len(recv_buf_split) <= 0 {
            log.Println("parse http err")
            return
        }

        header_data := string(recv_buf_split[0])
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
        //fmt.Printf("body_recved_len: %d\n", body_recved_len)
        //fmt.Printf("all: %d  | %d \n", body_recved_len + header_length, recv_len)
        //fmt.Printf("body: %s\n", recv_buf)

        // copy body data 
        for i:= 0; i < body_recved_len; i++ {
            body_data = append(body_data, recv_buf[header_length + i])
        }

        body_left_len := body_length - body_recved_len
        //fmt.Printf("body data: %s\n left_len %d\n", body_data, body_left_len)

        // left
        for true {
            if(body_left_len <= 0) {
                break
            }

            recv_len, err := l.io.Read(recv_buf)
            if err != nil {
                if(recv_len == 0) {
                    fmt.Printf("Host disconnected! err:%v\n", err)
                }
                return
            }

            body_left_len -= recv_len
            for i:= 0; i < recv_len; i++ {
                body_data = append(body_data, recv_buf[i])
            }
        }
        //fmt.Printf("body data: %s\n", body_data)
        l.read_chan <- body_data
    }
}

func (l *LgxSock) Read() []byte {
    body_data := <-l.read_chan
    return body_data;
}

func lgx_chat_menu() {
    fmt.Println("lgx chat client for golang")
}

func lgx_chat_login(lh LgxSock) {
    var vname string
    fmt.Println("Your name:")
    fmt.Scanln(&vname)
    s := RequestJson {
        Request: "login",
        Content : LoginInfo{
            Name : vname,
        },
    }
    data, err := json.Marshal(s)
    if err != nil {
       log.Fatal(err)
    } 
    lh.Post("/", "request=login", data)
    //fmt.Printf("send: %s\n", data)
    fmt.Printf("waiting ...")
    rdata := lh.Read() 

    fmt.Printf("recv: %s\n", rdata)
}

func lgx_chat_main() {
    lh := LgxSock{}
    lh.Init("127.0.0.1:80")
    go lh.Connect()
    // for delay start recving
    time.Sleep(time.Millisecond * 100)
    lgx_chat_login(lh)
    time.Sleep(time.Second * 100)
}

func main() {
    // handle exit
    sigs := make(chan os.Signal, 1) // register a exit signal
    //done := make(chan bool, 1) // func return channel
    signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
    go func() {
        sig := <-sigs   
        fmt.Println("Exit ...")
        fmt.Println(sig)
        //done <- true
        os.Exit(0)
    }()

    lgx_chat_main()

    /*
    cmd := exec.Command("whoami")
    var out bytes.Buffer
    cmd.Stdout = &out
    err := cmd.Run()
    if err != nil {
        log.Fatal(err)
    }
    fmt.Printf("cmd result:%s\n", out.String())
    */
}
