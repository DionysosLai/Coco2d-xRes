$(pageInit);
var jsonData = { articleId: 0 };
var xh = null;
function pageInit() {    
    xh = $('#editor').xheditor({
        plugins: getCodePlugin(),
        tools: "Cut,Copy,Paste,Pastetext,|,Blocktag,Fontface,FontSize,Bold,Italic,Underline,Strikethrough,FontColor,BackColor,Removeformat,|,Align,List,Outdent,Indent,|,Link,Unlink,Img,Emot,Table,Code,SCode,|,Source,Fullscreen",
        upImgUrl: "!/article/uploadimg",
        loadCSS: "http://csdnimg.cn/pig/blog/write/css/editor.css"
    });
    
    tags_init();
    homepage_check();
    if (jsonData.isDeleted) {
        topNote("该文章已被删除，无法编辑。");
        xh.setSource('');
    } else {
        edit_init();
    }
    if (jsonData.isClientUser) {
    	$('#p_desc,#d_desc').show();
    }
    
    $("#btnPublish").click(publish);
    $("#btnDraft").click(draft);
    $("#btnCancel").click(cancelSave);
    
    $("#selType").change(function () {
    	if (this.value == "2") {
    		$("#chkHomeDiv").hide();
    	} else {
    		$("#chkHomeDiv").show();
    	}
    });
    $('#chkHome').click(function () {
    	if (this.checked)
    		$('#p_desc,#d_desc').show();
    	else
    		$('#p_desc,#d_desc').hide();
    });

    window.onbeforeunload = function () {
        if (showConfirm) {
            if (csdn.val2("editor").replace(/<.+?>/g, "").length > 0) {
                try { return "您的文章尚未保存！"; } catch (err) { }
            }
        }
    };

    $("#imagecode,#seeagain").click(function () {
        checkcodeRefesh();
    });
}

function checkcodeRefesh()
{
    setTimeout(function () {
        var imagecode = $("#imagecode");
        if (imagecode.length > 0) {
            $("#imagecode").attr("src", "/image/index?r=" + Math.random());
        }       
    },500);    
}

function topNote(text) {
    var div = document.createElement("div");
    div.id = "top_note";
    div.className = "radius";
    div.innerHTML = text;
    document.body.appendChild(div);
    $("#txtTitle").blur();
    $(".btn_area_1 input").attr("disabled", true);
}
function getCodePlugin() {
    var codeArr1 = ["html", "objc", "javascript", "css", "php", "csharp", "cpp", "java", "python", "ruby", "vb", "delphi", "sql", "plain"];
    var codeArr2 = ["HTML/XML", "Objective-C", "JavaScript", "CSS", "PHP", "C#", "C++", "Java", "Python", "Ruby", "Visual Basic", "Delphi", "SQL", "其它"];
    var opts = '';
    for (var i = 0; i < codeArr1.length; i++) {
        opts += csdn.format('<option value="{0}">{1}</option>', codeArr1[i], codeArr2[i]);
    }
    var htmlCode = '<div>编程语言: <select id="xheCodeType">' + opts + '</select></div>'
        + '<div><textarea id="xheCodeCon" rows=6 cols=40></textarea></div>'
        + '<div style="text-align:right;"><input type="button" id="xheSave" value="确定" /></div>';
    var htmlSCode = '<div><textarea id="xheSCodeCon" rows=6 cols=40></textarea></div>'
        + '<div style="text-align:right;"><a href="https://code.csdn.net/help/CSDN_Code/code_support/FAQ_6_1" style="float:left;" target=_blank>什么是CODE代码片？</a><input type="button" id="xheSSave" value="确定" /></div>';

    var codePlugin = {
    	Code: {
    		c: 'Code', t: '插入代码', h: 1, e: function () {
    			var _this = this;
    			var jTest = $(htmlCode);
    			var jSave = $('#xheSave', jTest);
    			jSave.click(function () {
    				var sel_code = $("#xheCodeType").val();
    			    //var str = csdn.format('<pre name="code" class="{0}">{1}</pre><br />', sel_code, _this.domEncode($("#xheCodeCon").val()));
    				var xheCodeConContent = $("#xheCodeCon").val()+"";
    				xheCodeConContent = xheCodeConContent.replace(/&reg/g, "&amp;reg").replace(/&copy/g, "&amp;copy");

    				var str = '<pre name="code" class="' + sel_code + '">' + _this.domEncode(xheCodeConContent) + '</pre><br />';
    				_this.loadBookmark();
    				_this.pasteHTML(str);
    				_this.hidePanel();
    				document.cookie = "postedit_code=" + sel_code + "; expires=" + function () { var d = new Date(); d.setFullYear(d.getFullYear() + 1); return d.toGMTString(); }();
    				return false;
    			});
    			_this.saveBookmark();
    			_this.showDialog(jTest);
    			var _his_code = document.cookie.match(new RegExp("(^|\s)postedit_code=([^;]*)(;|$)"));
    			if (_his_code) $('#xheCodeType').val(_his_code);
    			else $("#xheCodeType option")[0].selected = true;
    		}
    	}
		, SCode: {
			c: 'SCode', t: '引用CODE代码片', h: 1, e: function () {
				var _this = this;
				var jTest = $(htmlSCode);
				var jSave = $('#xheSSave', jTest);
				jSave.click(function () {
					var str = $("#xheSCodeCon").val();
					var cid = 0;
					var mat = new RegExp('/([a-z0-9]+).js', 'i').exec(str);
					if (mat) { cid = mat[1]; }
					else if (/^[a-z0-9]+$/i.test(str)) { cid = str; }
					if (cid) {
						str = '<div>{CSDN:CODE:' + cid + '}</div>';
						_this.loadBookmark();
						_this.pasteHTML(str);
					}
					_this.hidePanel();
					return false;
				});
				_this.saveBookmark();
				_this.showDialog(jTest);
			}
		}
    };
    return codePlugin;
}
var showConfirm = true;


function publish() {   
    save(1);
}
function draft() {
    save(0);
    checkcodeRefesh();
}
var artId = 0;
var saveInter = null;
var saving = false; /*标识文章正在保存*/
function save(isPub) {
    if (csdn.doing) {/*有其它操作（如保存草稿），等其完成再保存*/
        setTimeout("save(" + isPub + ")", 500);
        return;
    }
    if (saving) {
        alert("文章正在保存，请耐心等待。");
        return;
    }
    
    if (!checkForm()) return;

    if (isPub) {/*发布后停止自动保存*/

        var txtCheckCode = $("#txtCheckCode");
        if (txtCheckCode.length > 0) {
            if ($.trim(txtCheckCode.val()) == "") {
                showErr("请输入验证码计算结果。");
                return false;
            }
        }

        if (saveInter) clearInterval(saveInter);
        saveInter = null;
    } else if (saveInter){/*如果是立即保存，重置定时器，避免同时保存2遍*/
        clearInterval(saveInter);
        saveInter = setInterval("autoSave()", 30 * 1000);
    }
    
    var data = getPostData();
    data += "&stat=" + (isPub ? "publish" : "draft");
    saving = true;
    showNote("正在保存。。。");
    var link = '?edit=1';
    if (isPub) {
        link += "&isPub=1";
    }
    link += "&r="+Math.random();
    $.ajax({
        type: 'POST',
        url: link,
        data: data,
        success: function (ret) {
            saving = false;
            ret = csdn.toJSON(ret);
            if (ret.result == 0) {
                showErr(ret.content);
                if (ret.content == "您输入的验证码计算结果不正确。") {
                    if ($("#checkcodearea").html().length < 5) {
                        var r = Math.random();
                        var checkcodehtml = '<div style="line-height:24px;">   ' +
                            '<span style="padding:0px 0 0px 10px;float:left;display:block;line-height:22px;">请输入计算结果（结果为数字）&nbsp;&nbsp;</span>' +
                             '<input type="text" id="txtCheckCode" style="width:5%; height:20px;float:left;" maxlength="4">' +
                             '<img src="/Image/Index?r=' + r + '" id="imagecode" style="padding:0px 0 0px 10px;float:left;" alt="验证码">  <a href="javascript:void(0);" id="seeagain" style="display:block;float:left;line-height:22px;margin-left:5px;">看不清</a>' +
                             '&nbsp;&nbsp;&nbsp;&nbsp;提示：自动保存和点击立即保存时不需要输入计算结果' +
                      '</div>';
                        $("#checkcodearea").html(checkcodehtml);
                    }
                    else {
                        checkcodeRefesh();
                    }
                }
                else {
                    checkcodeRefesh();
                }                          
            } else {
                showConfirm = false;
                if (!isPub) {
                    artId = ret.data;
                    showNote(csdn.format("文章已保存{0} {1}", (jsonData.articleId == '0' ? "为草稿" : ""), (new Date()).format("hh:mm:ss")));
                    $("#autosave_note").html('');
                } else {
                    saving = true; //保存后避免再次保存
                    showNote(ret.content + "正在跳转..."); // + "<a href='" + ret.data + "' target=_blank>点击查看</a>");
                    if (jsonData.articleId == 0) {/*如果不是编辑*/
                        $("#txtTitle").val('');
                        $("#editor").val('');
                    }
                    location = ret.data;                  
                }
            }
        },
        error: function () {
            saving = false;
            showNote('保存失败，请稍后重试。');
        }
    });
}
var old_con = null;
function autoSave() {
    if (csdn.doing || saving) return;
    var con = csdn.val2("editor");
    if (con.replace(/<.+?>/g, "").length < 100) return;
    if (con == old_con) return;
    old_con = con;
    csdn.doing = true;
    var data = getPostData() + "&stat=draft&isauto=1";
    $.ajax({
        type: 'POST',
        url: '?edit=1',
        data: data,
        success: function (ret) {
            csdn.doing = false;
            ret = csdn.toJSON(ret);
            if (ret.result == 1) {
                artId = ret.data;
                showConfirm = false;
                $("#autosave_note").html("文章已自动保存为草稿 " + (new Date()).format("hh:mm:ss"));
                showNote('');
            } else {
                /*showErr("文章自动保存失败，请注意保存。");*/
            }
        },
        error: function () {
            csdn.doing = false;
        }
    });
}
function getPostData() {    
    var type = $("#selType").val();
    var titl = csdn.val2("txtTitle");
    var cont = encodeURIComponent(csdn.val("editor").replace(/<a\s/gi, '<a target=_blank '));
    var desc = csdn.val2("txtDesc");
    var tags = csdn.val2("txtTag");
    var flnm = csdn.val2("txtFileName");
    var chnl = $("input[name=radChl]:checked").val() || 0;
    var comm = $("input[name=radComment]:checked").val();
    var leve = $("#chkHome").attr("checked") ? 1 : 0;
    var tag2 = encodeURIComponent(function () { var s = []; $('#d_tag2 span').each(function () { s.push(this.innerHTML); }); return s.join(','); }());
    GetResult();
    var data = "titl=" + titl + "&typ=" + type + "&cont=" + cont + "&desc=" + desc;
    data += "&tags=" + tags + "&flnm=" + flnm + "&chnl=" + chnl + "&comm=" + comm + "&level=" + leve;
    data += "&tag2=" + tag2;
    data += "&artid=" + artId;
    data += "&checkcode=" + $("#txtCheckCode").val();
    data += "&userinfo1=" + $("#userinfo2").val();

    return data;
}
function cancelSave() {
    showConfirm = false;
    if (jsonData.articleId != '0' && jsonData.isDraft == 'True')
        artId = jsonData.articleId;
    if (artId) {
        $.get("?del=1&artid=" + artId, function (ret) {
            location = "/postlist";
        });
    } else {
        location = "/postlist";
    }
}

function checkForm() {
    if ($("#selType").val()=='0') {
        showErr("请选择文章类型。");
        return false;
    }
    if (!csdn.hasVal("txtTitle")) {
        showErr("请输入文章标题。");
        return false;
    }
    var con = xh.getSource();
    if (!$.trim(con)) {
        showErr("请输入文章内容。");
        return false;
    }
    if ($("#chkHome").attr("checked")) {
        var chnl = $("input[name=radChl]:checked").val() || 0;
        if (!chnl) {
            showErr("发布到首页必需选择一个文章分类。");
            return false;
        }
    }  
   
    /*
    if (jsonData.expert && jsonData.expert == 'True') { }
    else {
        if (csdn.hasOutImg(con, hostList)) {
            showErr("不允许使用外站图片，请使用图片上传功能。");
            return false;
        }
    }*/

    return true;
}
/*提示信息*/
function showErr(err) {
    showMsg(err, true);
}
function showNote(msg) {
    showMsg(msg, false);
}
function showMsg(msg, isErr) {
    var srcMsg = msg;
    if (isErr) msg = "<font color=red>* " + msg + "</span>";
    else msg = "<font color=green>" + msg + "</font>";

    $("#sp_note").html(msg);

    if (srcMsg) $("#sp_note").show();
    else $("#sp_note").hide();
}
/*end*/

function edit_init() {
    if (jsonData.articleId == '0' || jsonData.isDraft == 'True') {
        /*自动保存策略：
        1、修改文章不自动保存（修改草稿自动保存）；
        2、点击发布按钮后不自动保存；
        3、文章内容字数小于100不自动保存；
        4、自动保存后离开页面不弹出提示；
        */
        saveInter = setInterval("autoSave()", 30 * 1000);
        $("#txtTitle").focus();
    } else {$("#p_n").show(); }
    if (jsonData.articleId != '0') {
        $("#txtTitle").focus().val(decodeURIComponent(jsonData.title));
        $("#selType").val(jsonData.type);
        $("#txtTag").val(decodeURIComponent(jsonData.tags)).blur();
        $("#txtTag2").val(decodeURIComponent(jsonData.tag2));formatTag2();
        $("#txtFileName").val(jsonData.fileName);
        $("input[name=radChl][value=" + jsonData.channel + "]").attr("checked", true);
        $("input[name=radComment][value=" + jsonData.comment + "]").attr("checked", true);

        if (jsonData.toHome == 'True') {
        	$("#chkHome").attr("checked", true);
        	$('#p_desc,#d_desc').show();
        }
        if (jsonData.isDraft != 'True') {
        	$("#p_comment,#d_comment").show();
        }
    }
}
function homepage_check() {
    if (jsonData.toHome && jsonData.toHome == 'True') {
        $("#sp_tohome_note").html(csdn.format("普通用户每天有<b>{0}</b>次机会发布到首页。专家无限制。", jsonData.tohome_max_cnt));
        return;
    }
    if (jsonData.unhome) {
        $('#chkHome').attr('disabled', true);
        $("#sp_tohome_note").html("文章被管理员撤出首页3天内不能再次发布到首页。");
        return;
    }
    if (jsonData.point && jsonData.point == 'False') {
        $('#chkHome').attr('disabled', true);
        $("#sp_tohome_note").html("博客积分需要大于<b>" + jsonData.max_point + "</b>才有权限发布到首页。");
    } else {
        if (jsonData.expert == "False") {
            $("#sp_tohome_note").html(csdn.format("每天有<b>{0}</b>次机会发布到首页，您今天已使用<b>{1}</b>次。专家无限制。", jsonData.tohome_max_cnt, jsonData.tohome_cnt));
            if (parseInt(jsonData.tohome_cnt) >= parseInt(jsonData.tohome_max_cnt))
                $('#chkHome').attr('disabled', true);
        } else {
            $("#sp_tohome_note").html(csdn.format("普通用户每天有<b>{0}</b>次机会发布到首页。专家无限制。", jsonData.tohome_max_cnt));
        }
    }
}
/*==tag start==*/
function tags_init() {
    if (!tags[0]) {
        tags = [];
        return;
    }
    $("#txtTag").autocomplete(tags);
    $("#txtTag").blur(resetChks);

    var s = [];
    var cols = 5;
    var tdString = '<td><input id="chk_tag_{0}" type="checkbox" onclick="chkTag(this)" /><label for="chk_tag_{0}">{1}</label></td>';
    s.push('<table id="tagtb" cellspacing="0">');
    //s.push('<thead>');
    s.push('<tr>');
    for (var i = 0; i < tags.length && i < cols; i++) {
        s.push(csdn.format(tdString, i, tags[i]));
    }
    s.push("</tr>");
    //s.push('</thead>');
    if (tags.length > cols) {
        //s.push('<tbody style="display:none;">');
        for (i = cols; i < tags.length; i++) {
            if (i % cols == 0) s.push('<tr>');
            s.push(csdn.format(tdString, i, tags[i]));
            if ((i + 1) % cols == 0) s.push('</tr>');
        }
        if (tags.length % cols > 0) {
            for (i = 0; i < cols - tags.length % cols; i++) {
                s.push('<td></td>');
            }
            s.push('</tr>');
        }
        //s.push('</tbody>');
    }
    s.push('</table>');
    if (tags.length > cols) {
        //s.push('<a id="tagall" href="javascript:void(0);" onclick="javascript:showAllTags(this);return false;">[全部]</a>');
    }
    $("#tagbox").html(s.join(''));

}
function chkTag(e) {
    var val = "";
    if (e.checked) {
        val = $("#txtTag").val().trim(',').trim('，') + ',' + $(e).next().text();
        val = val.trim(',').trim('，');
    } else {
        val = $("#txtTag").val().replace(new RegExp("(^|[,，])(" + $(e).next().text().replace(/([()])/g,'\\$1') + ")([,，]|$)", "ig"), function (all, s1, s2, s3) {
            return (s1 && s3) ? s1 : '';
        });
    }
    $("#txtTag").val(val.trim(','));
}
/*根据输入的tags重置选择框状态*/
function resetChks() {
    var arr = $("#txtTag").val().toLowerCase().split(/[,，]+/g);
    $("#tagbox input").each(function () {
        var has = false;
        var val = $(this).next().text().toLowerCase();
        for (var i = 0; i < arr.length; i++) {
            if (arr[i] == val) {has = true; break; }
        }
        this.checked = has;
    });
}
var valArray = ['', '', ''];
function getCurrVal(obj) {
    valArray[1] = '';
    var leftVal = getLeftText(obj);
    if (!leftVal) return "";
    var lastChar = leftVal.substr(leftVal.length - 1, 1);
    if (isSplit(lastChar)) return "";
    var allVal = obj.value;
    var i;
    for (i = leftVal.length - 1; i >= -1; i--) {
        if (i > -1 && isSplit(leftVal.substr(i, 1))) break;
    }
    if (i < -1) i = -1;
    valArray[1] = leftVal.substr(i + 1).trimStart();
    valArray[0] = leftVal.substr(0, i + 1);
    for (i = leftVal.length; i <= allVal.length; i++) {
        if (i < allVal.length && isSplit(allVal.substr(i, 1))) break;
    }
    valArray[2] = allVal.substr(i, allVal.length - i);
    
    return (valArray[1]);
}
function isSplit(c) {
    return (c == ',' || c == '，');
}
function getLeftText(obj) {
    if (obj.selectionStart || obj.selectionStart==0) {
        var idx = obj.selectionStart;
        return obj.value.substr(0, idx);
    }
    var rngSel = document.selection.createRange();
    var flag = rngSel.getBookmark();
    var rngTxt = obj.createTextRange();
    rngTxt.collapse();
    rngTxt.moveToBookmark(flag);
    rngTxt.moveStart('character', -obj.value.length);
    return rngTxt.text;
}
function setCurrVal(val) {
    if (!valArray[1] || !val) return;
    $("#txtTag").val(valArray[0] + val + valArray[2]);
}
function showAllTags(e) {
    if (e.innerHTML == "[全部]") {
        if ($("#tagbox").css("position") != "absolute") {
            var pos = $("#tagbox").position();
            $("#tagbox").css({ position: "absolute", left: pos.left + "px", top: pos.top+2 + "px" });
        }
        $("#tagtb tbody").show();
        e.innerHTML = "[收起]";
    } else {
        $("#tagtb tbody").hide();
        e.innerHTML = "[全部]";
    }
    e.blur();
}

function GetResult()
{
    var userinfo1 = $("#userinfo1").val();
    var d = userinfo1.split(' ');
    var dd = d[0].split('-');
    var ddd = d[1].split(':');
    var result =parseInt(dd[0]) + parseInt(dd[1]) + parseInt(dd[2]) +parseInt(ddd[0]) + parseInt(ddd[1]) + parseInt(ddd[2]);
    result = result % 60;
    var un = getcookie("UserName").toLowerCase();
    var ascresult = 0;
    for (var i = 0; i < un.length; i++)
    {
        ascresult += parseInt(getAsc(un.substr(i, 1)));
    }
    ascresult = ascresult % 60;    
    $("#userinfo2").val(ascresult * result);
}

function getAsc(str) {
    return str.charCodeAt(0);
}

function getcookie(name) {
    var cookie_start = document.cookie.indexOf(name);
    var cookie_end = document.cookie.indexOf(";", cookie_start);
    return cookie_start == -1 ? '' : unescape(document.cookie.substring(cookie_start + name.length + 1, (cookie_end > cookie_start ? cookie_end : document.cookie.length)));
}

/*==tag end==*/

/*==tag2 begin==*/

$(function () {
    $('.section').mouseout(getTag2);
    $('#txtTag2').focus(showTag2).blur(formatTag2).keydown(tagKeydown);
    $('#txtTag2,#tag2box').mouseover(function () {
    	window.overtag2 = true;
    }).mouseout(function () {
    	window.overtag2 = false;
    });
    $(document.body).click(function (ev) {
    	if (window.overtag2) return;
        $('#tag2box').hide();
    });
    utag2 = unique_sort(utag2).slice(0,10);
    //填充常用标签
    if (utag2[0] != '') {
        var s = '';
        for (var i = 0; i < utag2.length; i++) {
            s += '<a onclick="javascript:setTag2(this);return false;">' + utag2[i] + '</a>';
        }
        $('#td_tag21').html(s);
        activeTag2();
    }
    var selTypeValue = $("#selType").val();
    if (selTypeValue == "2") {
        $("#chkHomeDiv").hide();
    } else {
        $("#chkHomeDiv").show();
    }

    if ($("#selType").val() == "1") {
        $(".subtit span").first().show();
    } else {
        $(".subtit span").first().hide();
    }
    
    $("#selType").click(function() {
        if ($("#selType").val() == "1") {
            $(".subtit span").first().show();
        } else {
            $(".subtit span").first().hide();
        }
    });
});
var old_con_len = 0;
function getTag2() {
    var len = csdn.val2("editor").replace(/<.+?>/g, '').length;
    if (Math.abs(len - old_con_len) < 10) {
        return;
    }
    old_con_len = len;

    var data = { title: csdn.val("txtTitle"), body: csdn.val("editor").replace(/<.+?>/g, '') };
    $.post('?gettag=1', data, function (data) {
        var s = '';
        for (var i = 0; i < data.length; i++) {
            if (!utag2.has(data[i].name))
                s += '<a onclick="javascript:setTag2(this);return false;">' + data[i].name + '</a>';
        }
        if (s) {
            $('#td_tag22').html(s);
        }
    });
}

function showTag2() {
    if ($('#tag2box a').length > 0) {
        $('#tag2box').show();
    }
}
function setTag2(e) {
    var t = e.innerHTML;
    if (e.className == 'act') {
        $('#d_tag2 span').each(function () {
            if (this.innerHTML == t) this.click();
        });
        e.className = '';
    } else {
        $('#txtTag2').val(t);
        formatTag2();
    }
}
function tagKeydown(ev) {
    var code = (ev ? ev.which : event.keyCode);
    if (code == 8) {
        //如果是退格键
        var ss = $('#d_tag2 span');
        if (ss.length > 0 && $("#txtTag2").val().length == 0) {
            ss.eq(ss.length - 1).remove();
            formatTag2();
        }
    }
}
function formatTag2() {
    if ($('#txtTag2').val()) {
        var tt = '';
        $('#d_tag2 span').each(function () {
            tt += this.innerHTML + ',';
        });
        tt += $('#txtTag2').val().trim(',').replace(/[^\u4e00-\u9fa5\w\s\-+.#,，]+/g, '');
        tt = tt.trim(',').split(/[,，]+/g);
        var s = [];
        for (var i = 0; i < tt.length; i++) {
            if (!s.has(tt[i])) {
                s.push(tt[i].trim().slice(0, 20));
            }
            if (s.length == 5) break;
        }
        var str = '';
        for (var i = 0; i < s.length; i++) {
            str += '<span>' + s[i] + '</span>';
        }
        $('#d_tag2').html(str);
        $('#d_tag2 span').click(function () {
            $(this).remove();
            formatTag2();
        }).attr('title', '单击删除该标签');
    }
    var w = $('#d_tag2').width();
    $('#txtTag2').val('').css({
        'padding-left': w + 2,
        'width': 576 - w
    });
    activeTag2();
}
/*高亮预选标签中被选中的标签*/
function activeTag2() {
    var tags = [];
    $('#d_tag2 span').each(function () {
        tags.push(this.innerHTML);
    });
    $('#td_tag21>a,#td_tag22>a').each(function () {
        if (tags.has(this.innerHTML)) {
            this.className = 'act';
        } else {
            this.className = '';
        }
    });
}

Array.prototype.has = function (a) {
    for (var i = 0; i < this.length; i++) {
        if (this[i].toLowerCase() == a.toLowerCase())
            return true;
    }
    return false;
};
/*对tag进行去重排序*/
function unique_sort(arr) {
    arr.sort();
    var res = [[arr[0], 1]];
    for (var i = 1; i < arr.length; i++) {
        if (arr[i] != res[res.length - 1][0]) {
            res.push([arr[i], 1]);
        } else {
            res[res.length - 1][1]++;
        }
    }
    res.sort(function (a, b) { return a[1] < b[1]; });
    var aa = new Array();
    for (var i = 0; i < res.length; i++) {
        aa.push(res[i][0]);
    }
    return aa;
}
/*==tag2 end==*/