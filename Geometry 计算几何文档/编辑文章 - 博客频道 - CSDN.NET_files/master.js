function setTab() {
    var ctl = csdn.urls[1].split('?')[0];
    if (!ctl) ctl = "postlist";
    if (ctl == "postedit" || ctl == "import") {
        ctlTab(ctl);
        return;
    }
    if (csdn.urls.length > 2) {
        var u = location + '';
        ctl = u.substr(u.indexOf(ctl)).split('?')[0];
    }
    $("#ul_tab a").each(function () {
        if (this.href.endWith(ctl)) {
            this.parentNode.className = "active";
            return false;
        }
    });
}
function ctlTab(ctl) {
    $("#tab_" + ctl).show().attr("class", "active");
    $("#btn_" + ctl).hide();
}