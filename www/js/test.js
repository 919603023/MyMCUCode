var my_timer;

function timer_fun() {
    my_timer = setInterval("Get_NowTime()", 1000);
}

var timer;
var AutoRefreshFLag = "close";

function AutoRefreshTimer() {
    if (AutoRefreshFLag == "close") {
        timer = setInterval("refresh()", 2000);
        AutoRefreshFLag = "open";
        document.getElementById("autorefresh_button").value = "自动刷新";

    } else {
        timer = clearInterval(timer);
        document.getElementById("autorefresh_button").value = "手动刷新";
        AutoRefreshFLag = "close";
    }

}

//刷新函数
function refresh() {


    //从服务器获取数据
    
    var url="/cgi-bin/MyCgi.cgi?";
    url += "flush";
    //设置回调函数
    var xmlhttp = getXMLHttpRequest();
    xmlhttp.onreadystatechange=function () {
        if(xmlhttp.status==200 && xmlhttp.readyState==4)
        {
            var ret = xmlhttp.responseText;

            var data = ret;

            var Temp = data.substring(0, data.indexOf(":"));

            data = data.substr(data.indexOf(":") + 1);

            var Humi = data.substring(0, data.indexOf(":"));

            data = data.substr(data.indexOf(":") + 1);

            var Light = data.substring(0, data.indexOf(":"));

            data = data.substr(data.indexOf(":") + 1);

            var _1_Led_1 = data.substring(0, data.indexOf(":"));

            data = data.substr(data.indexOf(":") + 1);

            var Door = data;



            document.getElementById("Temp").innerText = Temp;

            document.getElementById("Humi").innerText = Humi;
            if (Light == "D") {
                document.getElementById("Light").innerText = "亮度过低";
            }
            else if (Light == "N") {
                document.getElementById("Light").innerText = "亮度正常";
            }
            else if (Light == "L") {
                document.getElementById("Light").innerText = "亮度过高";
            }
            if(Door == "N")
            {document.getElementById("Door").innerText = "正常";

            }
            else if(Door == "D"){
                document.getElementById("Door").innerText = "异常";
            }

            
            if (_1_Led_1 == "open") {

                document.getElementById("1_LED:1").value = "点击关闭";

            } else {

                document.getElementById("1_LED:1").value = "点击打开";
            }




        }
    }
    xmlhttp.open("GET",url, true);
    
    xmlhttp.send();
}


//按键按下时的处理函数
function AUTO_KeyClick() {
    if (window.event.keyCode == 13) {
        if (document.all('Register') != null) {
            document.all('Register').click();
        }
    }
}
//获取当前时间
function Get_NowTime() {

    var date = new Date();

    Y = date.getFullYear() + '-';

    M = (date.getMonth() + 1 < 10 ? '0' + (date.getMonth() + 1) : date.getMonth() + 1) + '-';

    D = date.getDate() + ' ';

    h = date.getHours() + ':';

    m = date.getMinutes() + ':';

    s = date.getSeconds();
    document.getElementById("date").innerHTML = Y + M + D + h + m + s;
    if (document.getElementById("date_id") != null)
        document.getElementById("date_id").value = Y + M + D + h + m + s;
}




$(function () {
    /*注意这里的鼠标事件，只给父菜单定义了一个mouseover，用来触发子菜单的显示，但不添加mouseout事件，否则鼠标还没移到子菜单上，它就消失了。*/

    $(".nav-portrait").mouseover(function () {
        $(".nav-second-ul").css('display', 'block');
    });

    $(".nav-second-ul").mouseover(function () {
        $(".nav-second-ul").css('display', 'block');
    });

    $(".nav-second-ul").mouseout(function () {
        $(".nav-second-ul").css('display', 'none');
    });
})




function getXMLHttpRequest() {
    var xmlhttp = null;
    if (window.XMLHttpRequest) //自动检测当前浏览器的版本，如果是 IE5.0 以上的高版本的浏览器
    { // code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest(); //创建请求对象
    } else ////如果浏览器是底版本的
    { // code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP"); //创建请求对象
    }
    return xmlhttp; //返回请求对象
}




function Ledcrtl(arg) {
    var xmlhttp = getXMLHttpRequest();
    var url = "/cgi-bin/MyCgi.cgi?";
    var data = "";
    var tmp = document.getElementById(arg).value;


    if (tmp == "点击打开") {
        data = "open";
    }
    else if (tmp == "点击关闭") {
        data = "close";
    }



    //设置回调函数
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.status == 200 && xmlhttp.readyState == 4) {
            var ret = xmlhttp.responseText;
           
            if (ret == "OK") {
                var tmp = document.getElementById(arg).value;
                if (tmp == "点击打开") {
                    document.getElementById(arg).value = "点击关闭";
                } else {
                    document.getElementById(arg).value = "点击打开";
                }
            }
        }
    }
    //open
    xmlhttp.open("GET", url + data, true);
    //send
    xmlhttp.send();


}


function JUMP(arg) {

    document.getElementById("one").innerHTML = "";
    document.getElementById('one').setAttribute('class', 'MyType');
    switch (arg) {

        case 0:

            var a = '<input type="button" class="active" value="注销" onclick="cancellation()" />\
                        <div class="container">\
                            <p class="title">\
                                <input type="button" class="data1" value="历史数据" onclick="HistoricalRecord()" />\
                                <input type="button" class="data2" id="date_id" value="        时间          " />\
                                <label id="device_id">1号棚</label>\
                                <input type="button" class="active" value="手动刷新" id="autorefresh_button" onclick="AutoRefreshTimer()" />\
                                <input type="button" class="active" value="刷新" onclick="refresh()" />\
                            </p>\
                            <div class="main">\
                                <div class="middle">\
                                    <h6>\
                                        <a>采集节点</a>\
                                        <a>当前值</a>\
                                    </h6>\
                                    <p>\
                                        <a>温度：</a>\
                                        <th><span id="Temp">10</span></th>\
                                        <label>℃</label>\
                                    </p>\
                                    <br>\
                                    <p>\
                                        <a>湿度：</a>\
                                        <th><span id="Humi">30</span></th>\
                                        <label>%</label>\
                                    </p>\
                                    <br>\
                                    <p>\
                                    <a>光照情况；</a>\
                                        <th><span id="Light"></span></th>\
                                    </p>\
                                    <br>\
                                    <br>\
                                    <p>\
                                    <a>门窗情况；</a>\
                                        <th><span id="Door"></span></th>\
                                    </p>\
                                    <br>\
                                    <br>\
                                    <br>\
                                </div>\
                                <div class="right">\
                                    <h6>\
                                        <a>控制节点</a>\
                                        <a>状态</a>\
                                    </h6>\
                                    <p>\
                                        <a>LED_1：</a>\
                                        <th><input type="button"  value="点击打开" id="1_LED:1" onclick="Ledcrtl(\'1_LED:1\')"></th>\
                                        <th></th>\
                                        <th>\
                                        </th>\
                                    </p>\
                                    <br>\
                                    <br>\
                                    ';
            document.getElementById("one").innerHTML = a;
            break;
        case 1:

            break;
        case 2:
            var a = '<p style="position:relative;top: 150px;"><br><label onclick="Jump(2);">想的美，没有帮助，自立更生</label></p>';
            document.getElementById("one").innerHTML = a;

            break;
        case 3:
            var a = '<p id="about" style="position:relative;top: 150px;"><br><label id="lab">这个系统，可好了，从来不会出错</label></p>';
            document.getElementById("one").innerHTML = a;
            Jump(3);
            break;
        case 4:
            var a = '\
            <div>\
            <br>\
            <br>\
            <label>ID</label>\
            <input placeholder="请输入ID" id="id" />\
            <br>\
            <br>\
            <label>旧密码</label>\
            <input placeholder="请输入旧密码" id="OldPwd" />\
            <br>\
            <br>\
            <label>新密码</label>\
            <input placeholder="请输入新密码" id="NewPwd" />\
            <br>\
            <br>\
            <input type="button" style="color: black; top: 40px;right: 20px; "value="确认更改" onclick="Register_Button(2) "></input>\
            </div>';
            document.getElementById("one").innerHTML = a;
            var temp = document.getElementById('one').removeAttribute('class');
            break;
        case 5:

            break;
        case 6:

            break;
        case 7:

            break;
        case 100:
            var a = '<h3>欢迎你</h3>\
            <form action="#" name="f" method="post">\
            <div class="input_outer">\
                <span class="u_user"></span>\
                <input name="logname" class="text" style="color: #FFFFFF !important" type="text" id="usr" placeholder="请输入账户">\
            </div>\
            <div class="input_outer">\
                <span class="us_uer"></span>\
                <input name="logpass" class="text" style="color: #FFFFFF !important; position:absolute; z-index:100;" value="" id="pwd" type="password" placeholder="请输入密码">\
            </div>\
            <div class="mb2"><a class="act-but submit" onclick="Register_Button(1)" id="Register" style="color: #FFFFFF">登录</a></div>\
            <input type="button" onclick="JUMP(101)" id="Register1" value="注册" >\
        </form>';
            document.getElementById("BOX").innerHTML = a;
            break;
        case 101:
            var a = '<h3>欢迎你</h3>\
            <form action="#" name="f" method="post">\
            <div class="input_outer">\
                <span class="u_user"></span>\
                <input name="logname" class="text" style="color: #FFFFFF !important" type="text" id="usr" placeholder="请输入账户">\
            </div>\
            <div class="input_outer">\
                <span class="us_uer"></span>\
                <input name="logpass" class="text" style="color: #FFFFFF !important; position:absolute; z-index:100;" value="" id="pwd" type="password" placeholder="请输入密码">\
            </div>\
            <div class="input_outer">\
                <span class="us_uer"></span>\
                <input name="logpass1" class="text" style="color: #FFFFFF !important; position:absolute; z-index:100;" value="" id="pwd1" type="password" placeholder="请再次输入密码">\
            </div>\
            <div class="mb2"><a class="act-but submit" onclick="Register_Button(3)" id="Register" style="color: #FFFFFF">注册</a></div>\
            <input type="button" onclick="JUMP(100)" id="Register1" value="返回" >\
        </form>';
            document.getElementById("BOX").innerHTML = a;
            break;
    }

}
/**************关于的文字炫彩效果*************************** */
var amy_timer;

function Jump(arg) {
    if (arg == 3)
        amy_timer = setInterval("my_fun()", 100);
    else if (arg == 2)
        window.location = "http://www.baidu.com";
}

function my_fun() {
    var div = document.getElementById("lab");
    div.style.color = div.style.color == 'red' ?
        'green' : (div.style.color == 'green' ? 'blue' : 'red');
}
/**************结束**************************************** */
function HistoricalRecord() {
    var xmlhttp = getXMLHttpRequest()
    var url = "/cgi-bin/MyCgi2.cgi";
    var data = "GETHistoricalRecord";

    //设置回调函数
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.status == 200 && xmlhttp.readyState == 4) {
            var ret = xmlhttp.responseText;
            alert(ret);



            var Html = "<table >";
            Html += " <tr>";
            var tmp = ret;
            var arr = new Array();
            var arr1 = new Array();

            tmp = tmp.substr(0, tmp.indexOf("*") + 1);

            var i = 0;
            while (tmp.indexOf("-") != -1) {
                arr[i] = tmp.substr(0, tmp.indexOf("-"));
                tmp = tmp.substr(tmp.indexOf("-") + 1);
                i++;
            }
            arr[i] = tmp.substr(0, tmp.indexOf("*"));
            tmp = ret;
            i = 0;
            tmp = tmp.substr(tmp.indexOf("*") + 1);
            while (tmp.indexOf("*") != -1) {
                arr1[i] = tmp.substr(0, tmp.indexOf("*"));
                tmp = tmp.substr(tmp.indexOf("*") + 1);
                i++;
            }
            arr1[i] = tmp;
            arr.shift();
            arr1.shift();
            alert(arr.length);
            alert(arr1.length);
            for (var i = 0; i < arr.length; i++) {

                alert(arr[i]);

            }
            for (var i = 0; i < arr1.length; i++) {

                alert(arr1[i]);

            }






            for (var i = 0; i < arr.length; i++) {

                Html += "<td>" + arr[i] + "</td>";

            }
            Html += "</tr>";
            for (var i = 0; i < arr1.length / arr.length; i++) {
                Html += "<tr>";

                Html += "<td>" + arr1[i * arr.length + 1] + "</td>";
                Html += "<td>" + arr1[i * arr.length] + "</td>";
                Html += "</tr>";
            }
            document.getElementById("one").innerHTML = Html;


        }



    }
    xmlhttp.open("POST", url, true);
    xmlhttp.send(data);
}
