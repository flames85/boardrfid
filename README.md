# boardrfid

1. win编程, vs2010编译良好. 


# 有关串口通信

1. 串口通信(类SerialPort)可单独分离. 需者自取.

2. 逻辑大概是从串口中读取一个字符串, 解析后保存进内存. 

# 有关ActiveX

1. 新建ActiveX方法是: 文件 | 新建 | Visual C++ | MFC | MFC ActiveX Control
   看不懂工程里一堆文件哪来的,可以新建一个试一试.

2. 如何查看工程API: 
   
   法1: 视图 | 类视图 | boardrifdLib | _Dboardrfid 
        (顺便一提,需要添加API的方法是右键_Dboardrfid, Add | Add Method)

   法2: 文件boardrfid.idl

    [id(1)] LONG InitPort(LONG nCom);
    
    [id(2)] LONG OpenListenThread(void);
    
    [id(3)] BSTR TakeOneRfid(void);
    
    [id(4)] LONG SetBufferSize(LONG nSize);
    
    [id(5)] LONG Init(LONG nCom, LONG nBaudRate);
    
   
3. 而实现,可以看文件boardrfidCtrl.cpp

4. 既然有了API如何调用?

首先注册编译生成的ocx, Regsvr32 xxx.ocx(反注册加/u)
然后在IE中执行如下文本:
<OBJECT id="ActiveX" CLASSID="CLSID:41B20F36-8986-40D4-9D16-E3466B13F386" ></OBJECT>
<script type="text/javascript">

ActiveX.Init(4, 57600);
ActiveX.OpenListenThread();
ActiveX.SetBufferSize(2);

</script>
<input type ="button" onclick ="alert(ActiveX.TakeOneRfid());" value ="RFID" />

5. "CLSID:41B20F36-8986-40D4-9D16-E3466B13F386"如何获取?
搜索Class information for , 下面一行uuid(41B20F36-8986-40D4-9D16-E3466B13F386)即是.
