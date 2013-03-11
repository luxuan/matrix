<%@ Page Language="C#" MasterPageFile="~/MasterPage.master" AutoEventWireup="true" CodeFile="Index.aspx.cs" Inherits="Index" Title="无标题页" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" Runat="Server">
    <link href="chart/csschart.css" rel="stylesheet" type="text/css" media="screen" />
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">

<%--<asp:Label ID="Label1" runat="server" Text="Label"></asp:Label>
<asp:TextBox ID="TextBox1" runat="server"></asp:TextBox>
<br />
<asp:Label ID="Label2" runat="server" Text="Label"></asp:Label>
<asp:TextBox ID="TextBox2" runat="server"></asp:TextBox>
<br />
<asp:Label ID="Label3" runat="server" Text="Label"></asp:Label>
<asp:TextBox ID="TextBox3" runat="server"></asp:TextBox>
<br />
<asp:Label ID="Label4" runat="server" Text="Label"></asp:Label>
<asp:TextBox ID="TextBox4" runat="server"></asp:TextBox>
<br />
<asp:Button ID="Button1" runat="server" Text="启动" onclick="Button1_Click" />--%>
<%--   <div>
 <br /> <br />    
<h2>各算法所用时间</h2>
<div class="yAxis" id="yDirect2" runat="server">
	
</div>
<div id="ob2" runat="server">	
   </div>
<div class="xAxis" runat="server" id="algName2">	
	
</div>
</div>
<br /> <br />  
<br /> <br />  
<br /> <br /> --%>   
<div style="float:left; width:100%">
<h2>各算法加速比</h2>
<div class="yAxis" id="yDirect" runat="server">
	
</div>
<div id="ob" runat="server">	
   </div>
<div class="xAxis" runat="server" id="algName">	
	
</div>
</div>

</asp:Content>

