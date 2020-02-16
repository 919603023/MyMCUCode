package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
public class MainActivity extends AppCompatActivity {
    Button ConnectButton;//定义连接按钮
    Button FlushButton;//定义发送按钮

    EditText MsgText;//定义信息输出框

    Socket socket = null;//定义socket
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ConnectButton = (Button) findViewById(R.id.Button_Connect);//获得按钮对象
        FlushButton = (Button) findViewById(R.id.Button_Flush);//获得按钮对象

    }
    public void Connect_onClick(View v) {

//启动连接线程
        Connect_Thread connect_Thread = new Connect_Thread();
        connect_Thread.start();
    }


    class Connect_Thread extends Thread//继承Thread
    {
        public void run()//重写run方法
        {
            try
            {
                if (socket == null) //如果已经连接上了，就不再执行连接程序
                {
                                       socket = new Socket("129.211.89.48", "8000");//创建连接地址和端口-------------------这样就好多了
                }
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }
        }
    }
}
