const { Query, User } = AV;
AV.init({
    appId: "Wbd8KGCPsnTmychbXwSTxriI-gzGzoHsz",
    appKey: "sMEYJaBGyoNbsfa6L5XGGECJ",
    serverURL: "https://wbd8kgcp.lc-cn-n1-shared.com"
});
function goLogin(){
    var id=document.getElementById("username").value;
    var pw=document.getElementById("password").value;
    login(id,pw);
}
function login(user_id, user_password) {
    const userQuery = new AV.Query("Account");
    userQuery.equalTo("userId", user_id);
    userQuery.find().then((usersGet) => {
        if (usersGet.length == 0) {
            // 用户id输错的情况
            console.log("用户不存在！");
            alert("用户不存在，请重新输入！");
        } else {
            if (usersGet[0].get("userPassword") != user_password) {
                // 用户密码输入错误的情况
                console.log("密码错误");
                alert("密码错误，请重新输入！");
            } else {
                // 用户id和密码一致的情况
                console.log("登录成功");

                // 登陆成功应该执行的代码
                // 检查是否是管理员
                var userGet = usersGet[0];
                if (userGet.get("userType") == 0) {
                    // 学生
                    location.assign("MainPage_Student.html?user=" + userGet.get("objectId"))
                } else {
                    // 管理员
                    location.assign("MainPage_Admin.html?user=" + userGet.get("objectId"))

                }
                
            }
        }
    });
}
