<%@ Control Language="C#" AutoEventWireup="true" CodeFile="Chart.ascx.cs" Inherits="admin_ascx_Chart" %>

<script type="text/javascript" src="chart/chart.js" ></script>
<link href="chart/csschart.css" rel="stylesheet" type="text/css" media="screen" />
<div class="chartBody">
    <asp:Panel ID="YPanel" CssClass="yAxis" runat="server" />
    <asp:Panel ID="ChartPanel" CssClass="csschart" runat="server" />
    <asp:Panel ID="XPanel" CssClass="xAxis" runat="server" />
    <div class="introdution">
        <asp:Label id="Info" runat="server" />
    </div>
</div>


