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

public partial class admin_ascx_Chart : System.Web.UI.UserControl
{
    // 若是多页的情况，自动调整高度将会比例不一致
    private bool _auto = false;  //是否自动调整高度
    private string _yAppend = null;
    private string _introdution = null;
    private ArrayList _xAxis = null;    // string
    private ArrayList _yValue = null;   // float
    public bool Auto
    {
        set { this._auto = value; }
    }
    public string YAppend
    {
        set { this._yAppend = value; }
    }
    public string Introdution
    {
        set { this._introdution = value; }
    }
    public ArrayList XAxis
    {
        set { this._xAxis = value; }
    }
    public ArrayList YValue
    {
        set { this._yValue = value; }
    }

    protected void Page_Load(object sender, EventArgs e)
    {
        const int LowestRate = 4;
        float step = 10;
        int low = 0;

        // 若要求自动调整，将最大值的高度设在50%-100%;
        if (_auto)
        {
            float max = 0, min = 0;
            foreach(float key in _yValue)
            {
                if (key > max)
                    max = key;
                else if (key < min)
                    min = key;
            }
            if (max > 100)
            {
                while (max>100)
                {
                    step *= 2;
                    max /= 2;
                }
            }
            else if (max > 0&&max<50)
            {
                double slip = (max - min) / 8.0;
                step = (int)slip + 1;
                low = (int)(min - step);
                if (low < 0)
                    low = 0;
            }
        }

        // 初始化纵坐标尺度;
        float i = low + 10 * step;
        for (; i > 0; i -=step )
        {
            Label labely = new Label();
            labely.CssClass = "yCssli";
            labely.Text = i + _yAppend;
            YPanel.Controls.Add(labely);
        }

        // 若含有超链接，则启用超链接,待完善;
        foreach (string x in _xAxis)
        {
            Label labelx = new Label();
            labelx.Text = x;
            labelx.CssClass = "xCssli";
            XPanel.Controls.Add(labelx);

        }

        // 初始化条形图高度;
        foreach (float yValue in _yValue)
        {
            Label labelc = new Label();
            Panel panelc = new Panel();

            labelc.CssClass = "chartContent";
            double th = low + (yValue - low) * 10 / step;
            labelc.Style.Add("height", th + "%");
            if(th>LowestRate)
                labelc.Text = "<b>" + yValue + "</b>";

            panelc.CssClass = "csssimple";
            panelc.Controls.Add(labelc);
            ChartPanel.Controls.Add(panelc);
        }
        Info.Text = _introdution;
    }
}
