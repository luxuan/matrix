using System;
using System.Collections;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Xml.Linq;
using System.Diagnostics;
using System.Text;


public partial class Index : System.Web.UI.Page
{
    protected string[] name;
    protected double[] time;
    protected double[] speed_up;
    protected int n;
    protected int m;
    protected void Page_Load(object sender, EventArgs e)
    {
       // System.IO.FileStream File = new System.IO.FileStream("Log.txt", System.IO.FileMode.CreateNew);
        //System.IO.StreamWriter sw = new System.IO.StreamWriter(File);
        /*
        System.IO.StreamWriter sw = System.IO.File.AppendText(Server.MapPath("text.txt"));
        sw.WriteLine("this is a test");
        sw.Flush();//清理缓冲区，将缓冲数据写入基础设备
        sw.Close();
        Response.Write("<script>alert('写入成功')</script>");
         * */
        n = 0;
        m = 0;
        int i;
        string content;
        double add;
        //System.IO.StreamReader sw = System.IO.File.OpenText(Server.MapPath("result.txt"));
        System.IO.StreamReader sw = new System.IO.StreamReader("E:\\project\\thread\\Matrix_lius\\Matrix\\result.txt", Encoding.GetEncoding("GB2312"));
        content=sw.ReadToEnd();// WriteLine("this is a test");
        //sw.Flush();//清理缓冲区，将缓冲数据写入基础设备
        sw.Close(); 
        
       
        string[] sArray = content.Split('\n');
        
        
        int row = Convert.ToInt32(sArray[0]);
        name = new string[row];
        time = new double[row];

        /////////////////////////////////////////////////////////////////////////////////
        //////打印时间图表
        double maxtime=0;
        double mintime=9000000;
        for ( i = 0; i < row; i++)
        {
            string[] con = sArray[i + 1].Split(':');
            name[i] = con[0];
            time[i] = Convert.ToDouble(con[1]);
            if (time[i] > maxtime)
                maxtime = time[i];
            else if (time[i] < mintime)
                mintime = time[i];
        }
        /*
        add = (maxtime - mintime) / 8;
        maxtime = maxtime + add;
        mintime = mintime - add;
        yDirect2.InnerHtml = "";
        for (i = 0; i < 10; i++)
        {
            yDirect2.InnerHtml = yDirect2.InnerHtml + "<div class='yCssli'>" + Math.Round((maxtime - add * i), 2) + "</div>";
        }
        yDirect2.InnerHtml = yDirect2.InnerHtml + "<div class='yCssli'>" + Math.Round((maxtime - add * i), 2) + "</div>";
        ob2.InnerHtml = "<div class='csschart' >";
        for (i = 0; i < row; i++)
        {

            //obj.InnerHtml = "<b>"+speed_up[i]+"</b>";
            //obj.Attributes.Add("style", "height:"+speed_up[i] * 10 + "%");
            double hight = Math.Round((time[i] - mintime) / add * 10, 2);
            ob2.InnerHtml = ob2.InnerHtml + "<div class='csssimple' ><span style='height:" + hight + "%' >" + Math.Round(time[i], 2) + "</span></div>";

        }
        ob2.InnerHtml = ob2.InnerHtml + "</div>";
        algName2.InnerHtml = "";
        for (i = 0; i < row; i++)
        {
            algName2.InnerHtml = algName2.InnerHtml + "<div class='xCssli'>" + name[i] + "</div>";
        }
        */

        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////打印加速比图表
        speed_up = new double[row];
        double maxSpeed=0;
        double minSpeed=9000000;
        for ( i = row-1; i >=0; i--)
        {
            speed_up[i] = time[0] /time[i];
            if (speed_up[i] > maxSpeed)
                maxSpeed = speed_up[i];
            else if(speed_up[i]<minSpeed)
                minSpeed = speed_up[i];
        }
        add = (maxSpeed - minSpeed) / 8;
        //double min = minSpeed - add;
        //csschart.Attributes.Add("background", "bg_chart.gif");
        //HtmlGenericControl obj;
        //obj = (HtmlGenericControl)(csschart.Controls[i + 1]);
        maxSpeed = maxSpeed + add;
        minSpeed = minSpeed - add;
        yDirect.InnerHtml = "";
        for (i = 0; i < 10; i++)
        {
            yDirect.InnerHtml = yDirect.InnerHtml + "<div class='yCssli'>" + Math.Round((maxSpeed - add * i), 2) + "</div>";
        }
        yDirect.InnerHtml = yDirect.InnerHtml + "<div class='yCssli'>" + Math.Round((maxSpeed - add * i), 2) + "</div>";
        ob.InnerHtml = "<div class='csschart' >";
        for (i = 0; i < row; i++)
        {

            //obj.InnerHtml = "<b>"+speed_up[i]+"</b>";
            //obj.Attributes.Add("style", "height:"+speed_up[i] * 10 + "%");
            double hight = Math.Round((speed_up[i] - minSpeed) / add * 10, 2);
            ob.InnerHtml = ob.InnerHtml + "<div class='csssimple' ><span style='height:" + hight + "%' >" + Math.Round(speed_up[i], 2) + "</span></div>";

        }
        ob.InnerHtml = ob.InnerHtml + "</div>";
        algName.InnerHtml = "";
        for (i = 0; i < row; i++)
        {
            algName.InnerHtml = algName.InnerHtml + "<div class='xCssli'>" + (i+1)+"."+name[i] + "</div>";
        }
         
    }
    /*
    protected void graph(string [] names,double [] num,int row,double max,double min,string idy,string idname,string idnum)
    {
        double add = (maxSpeed - minSpeed) / 8;
        //double min = minSpeed - add;
        //csschart.Attributes.Add("background", "bg_chart.gif");
        //HtmlGenericControl obj;
        //obj = (HtmlGenericControl)(csschart.Controls[i + 1]);
        maxSpeed = maxSpeed + add;
        minSpeed = minSpeed - add;
        yDirect.InnerHtml = "";
        for (i = 0; i < 10; i++)
        {
            yDirect.InnerHtml = yDirect.InnerHtml + "<div class='yCssli'>" + Math.Round((maxSpeed - add * i), 2) + "</div>";
        }
        yDirect.InnerHtml = yDirect.InnerHtml + "<div class='yCssli'>" + Math.Round((maxSpeed - add * i), 2) + "</div>";
        ob.InnerHtml = "<div class='csschart' >";
        for (i = 0; i < row; i++)
        {

            //obj.InnerHtml = "<b>"+speed_up[i]+"</b>";
            //obj.Attributes.Add("style", "height:"+speed_up[i] * 10 + "%");
            double hight = Math.Round((speed_up[i] - minSpeed) / add * 10, 2);
            ob.InnerHtml = ob.InnerHtml + "<div class='csssimple' ><span style='height:" + hight + "%' >" + Math.Round(speed_up[i], 2) + "</span></div>";

        }
        ob.InnerHtml = ob.InnerHtml + "</div>";
        algName.InnerHtml = "";
        for (i = 0; i < row; i++)
        {
            algName.InnerHtml = algName.InnerHtml + "<div class='xCssli'>" + name[i] + "</div>";
        }
    }
     * */
    protected void Button1_Click(object sender, EventArgs e)
    {
        string appName = "D:\\jzw\\blank\\Matrix_lius\\Debug\\Matrix.exe";
        try
        {
            Process proc = Process.Start(appName);
            if (proc != null)
            {
               // proc.WaitForExit(3000);
                if (proc.HasExited)
                    //MessageBox.Show(String.Format("外部程序 {0} 已经退出！", this.appName), this.Text,
                    //MessageBoxButtons.OK, MessageBoxIcon.Information);
                    ;
                else
                {
                    // 如果外部程序没有结束运行则强行终止之。

                    proc.Kill();
                    //MessageBox.Show(String.Format("外部程序 {0} 被强行终止！", this.appName), this.Text,
                    //MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }
            }
        }
        catch (ArgumentException ex)
        {
            //MessageBox.Show(ex.Message, this.Text,
                //MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}
