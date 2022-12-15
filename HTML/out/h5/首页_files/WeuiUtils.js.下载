var WeuiUtils = {
	// 获取select 对象
	getPickerList (arrList) {
		let arrNew = new Array();
		arrList.forEach((item, index) => {
			let obj = {};
			obj.label = item.sName;
			obj.value = item.sCode;
			arrNew.push(obj);
		})
		return arrNew;
	},
	// 检测当前浏览器是否 微信浏览器
	isWeiXin () {
		let ua = navigator.userAgent.toLowerCase(); 
		if(ua.match(/MicroMessenger/i)=="micromessenger") { 
		return true; 
		} else { 
		return false; 
		} 
	},
	// 判断 非微信浏览器 处理
	checkWeiXin () {
		WeuiUtils.isWeiXin() == false ? window.location.href = "../wxerror.html" : "";
	},
	// 指定位置转成星号,  frontLen：前面保留位数，endLen：后面保留位数。
	plusXing (str, frontLen, endLen) {
		var len = str.length-frontLen-endLen;
		var xing = '';
		for (var i=0;i<len;i++) {
			xing+='*';
		}
		return str.substr(0,frontLen)+xing+str.substr(str.length-endLen);
	},
	// 获取base64图片大小
	getBaseSize (base64url) {
		var baseStr;
		if (base64url.indexOf("data:image/png;base64,") != -1) { baseStr = base64url.replace('data:image/png;base64,', '') }
		if (base64url.indexOf("data:image/jpg;base64,") != -1) { baseStr = base64url.replace('data:image/jpg;base64,', '') }
		if (base64url.indexOf("data:image/jpeg;base64,") != -1) { baseStr = base64url.replace('data:image/jpeg;base64,', '') } 

		var egTagIndex = baseStr.indexOf("=");
		baseStr = egTagIndex != -1 ? baseStr.substring(0, egTagIndex) : baseStr;
		var strLen = baseStr.length;
		var fileSize = strLen-(strLen/8)*2;
		return fileSize;
	},
	// 单按钮弹窗
	oneBtnAlert (val, isClose, title) {
		weui.alert(val || "网络异常", {
			title: title || "提示",
			buttons: [{
				label: '确认',
				type: 'primary',
				onClick: function(){ 
					if (isClose == true) {
						wx.closeWindow();
					}
				}
			}]
		});
	},
	// 弹窗-刷新页面
	refreshPage () {
		weui.alert("页面出错，请检查网络或授权！", {
			buttons: [{
				label: '刷新页面',
				type: 'primary',
				onClick: function(){ window.location.reload(); }
			}]
		});
	},
	// 显示loading
	isShowLoading (type, text) {
		if(type) {
			$(".loading_mask").show();
			$(".loading_tip").css("display","flex");
			text && $(".loading_tip .loading_text").text(text);
		} else {
			$(".loading_mask").hide();
			$(".loading_tip").hide();
		}
	},
	// 成功时的提示
	isShowToast (val, time) { // val - 内容  time - 提示显示时间
		weui.toast(val, time || 2500);
	},
	//配置微信SDK
	configWxSDK (url, iWxSdkFlog, bForceGet, wxSdkApiList) { 
		let sSignUrl = window.location.href.split('#')[0];
		var wxSdkApiList = wxSdkApiList || ['checkJsApi','chooseImage','uploadImage','openLocation','getLocation',"hideMenuItems","showMenuItems","closeWindow",'hideAllNonBaseMenuItem','previewImage'];
		$.ajax({
			url: url,
			method: "post",
			data: {
				sSignUrl: sSignUrl,
				bForceGet: bForceGet
			},
			success: function(data) {
				console.log(iWxSdkFlog);
				iWxSdkFlog += 1;
				console.log(iWxSdkFlog);
				console.log("wx.config() ---> 接收后台返回的参数" + data);
				let config = data.data;
				wx.config({
					debug: false,
					appId: config.appid,
					timestamp: config.timestamp,
					nonceStr: config.nonceStr,
					signature: config.signature,
					jsApiList: wxSdkApiList
				});
				wx.ready(function (res) {
					// callback && callback();
				});
				wx.error(function(res) {
					// callback && callback();
					//config信息验证失败会执行error函数，如签名过期导致验证失败
					console.log(iWxSdkFlog);
					if(iWxSdkFlog <= 1) {
						console.log("触发了");
						//再配置一次
						WeuiUtils.configWxSDK(url, iWxSdkFlog, true);
					} else {
						window.location.href = "../wxerror.html";
					}
				});
			}
		});
	},
	// 获取人员类型
	getPersonType (iPersonType) {
	  let sPersonType = "";
	  switch (iPersonType) {
	    case 1:
	      sPersonType = "学生";
	      break;
	    case 2:
	      sPersonType = "老师";
	      break;
	    case 3:
	      sPersonType = "B岗/合同工";
	      break;
	    case 4:
	      sPersonType = "老师家属";
	      break;
	    case 5:
	      sPersonType = "B岗/合同工家属";
	      break;
	    case 6:
	      sPersonType = "住户及家属";
	      break;
	    case 7:
	      sPersonType = "租户及家属";
	      break;
	    case 8:
	      sPersonType = "商铺人员";
	      break;
	    case 9:
	      sPersonType = "幼小学生接送人员";
	      break;
	    case 10:
	      sPersonType = "外单位工勤人员";
	      break;
	    case 11:
	      sPersonType = "合作单位人员";
	      break;
	    case 12:
	      sPersonType = "钢研所人员";
	      break;
	    case 13:
	      sPersonType = "冶金公司人员";
	      break;
	    case 14:
	      sPersonType = "访客";
	      break;
	    case 15:
	      sPersonType = "其他";
	      break;
	  }
	  return sPersonType;
	},
	// 根据经纬度计算距离，参数分别为第一点的纬度，经度；第二点的纬度，经度
	getDistance (lat1, lng1, lat2, lng2) {
		var radLat1 = this.rad(lat1);
		var radLat2 = this.rad(lat2);
		var a = radLat1 - radLat2;
		var b = this.rad(lng1) - this.rad(lng2);
		var s = 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(a / 2), 2) +
			Math.cos(radLat1) * Math.cos(radLat2) * Math.pow(Math.sin(b / 2), 2)));
		s = s * 6378.137; // EARTH_RADIUS;
		s = Math.round(s * 10000) / 10000; //输出为公里
	
		var distance = s;
		var distance_str = "";
	
		if (parseInt(distance) >= 1) {
			distance_str = distance.toFixed(1) + "km";
		} else {
			distance_str = distance * 1000 + "m";
		}
	
		//s=s.toFixed(4);
		// console.info('lyj 距离是', s);
		// console.info('lyj 距离是', distance_str);
		return s;
	},
	// 经纬度转换成三角函数中度分表形式。
	rad (d) {
		return d * Math.PI / 180.0; 
	},
	// 表单验证
	formVerify (value, type) { // isMobile - 手机号格式
		console.log(value);
		console.log(type);
		
		// 检验是否手机格式 val:手机格式
		if (type == "isMobile") {
			if (!new RegExp("^[1]([3-9])[0-9]{9}$").test(value)) {
				return '亲，手机号格式有误~';
			}
		}
		
		// 检验是否学号 val:学号
		if (type == "isPersonCode") {
			if (!new RegExp("^[A-Za-z0-9]$").test(value)) {
				return '亲，学号格式有误~';
			}
		}
		
		// 检验是否中文数字字母 
		if (type == "isHZOrEnglishOrNum") {
			if (!new RegExp("^[\u4E00-\u9FA5A-Za-z0-9]$").test(value)) {
				return '亲，格式要求为中文数字字母~';
			}
		}

		// if(!new RegExp("^[a-zA-Z0-9_\u4e00-\u9fa5\\s·]+$").test(value)){
		// 	return '用户名不能有特殊字符';
		// }
		
		return true
	},
}