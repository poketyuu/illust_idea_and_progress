var MaxSid = 1;
function SetOrderEvent(sid) {
    MaxSid = sid;
    var StateList = document.getElementById("state-list");
    var sortable = Sortable.create(StateList,
        {
            animation: 150,
            ghostclass: 'blue-background-class',
            handle: '.row .col-auto .row .slide-btn'
        });
}
function AddState() {
    var NewStateTxt = document.getElementById("AddState_Txt");
    var NewState = NewStateTxt.value;
    if (NewState == "") return;
    NewStateTxt.value = "";
    // liの中身を生成
    var NewStateLi = document.createElement("li");
    NewStateLi.setAttribute("class", "list-group-item state-item")
    MaxSid++;
    NewStateLi.setAttribute("id", "state-" + MaxSid);
    NewStateLi.setAttribute("data-sid", -1);
    //通常表示用
    var row1 = document.createElement("div");
    row1.setAttribute("class", "row align-items-center justify-content-between")
    row1.setAttribute("id", "dispstate-" + MaxSid);
    //左部分(ハンバーガーアイコン,State名)
    var col1 = document.createElement("div");
    col1.setAttribute("class", "col-auto");
    var row2_1 = document.createElement("div");
    row2_1.setAttribute("class", "row align-items-center");
    //ハンバーガーアイコン
    var col1_1 = document.createElement("div");
    col1_1.setAttribute("class", "col-auto slide-btn mx-2")
    for (var i = 0; i < 3; i++) {
        var hamburger_span = document.createElement("span");
        hamburger_span.setAttribute("class", "hamburger-bar");
        col1_1.appendChild(hamburger_span);
    }
    row2_1.appendChild(col1_1);
    //State名
    var col1_2 = document.createElement("div");
    col1_2.setAttribute("class", "col-auto");
    var p_state = document.createElement("p");
    p_state.setAttribute("class", "my-auto state-name");
    p_state.setAttribute("id", "sname-" + MaxSid);
    p_state.textContent = NewState;
    col1_2.appendChild(p_state);
    row2_1.appendChild(col1_2);
    col1.appendChild(row2_1);
    row1.appendChild(col1);
    //右部分(編集,削除ボタン)
    var col2 = document.createElement("div")
    col2.setAttribute("class", "col-auto");
    var row2_2 = document.createElement("div");
    row2_2.setAttribute("class", "row align-items-center");
    //編集ボタン
    var col2_1 = document.createElement("div");
    col2_1.setAttribute("class", "col-auto");
    var editbtn = document.createElement("button");
    editbtn.setAttribute("class", "btn bnt-sm btn-outline-secondary");
    editbtn.setAttribute("onclick","ChangeEditMode(true,"+MaxSid+")")
    editbtn.textContent = "名前を変更";
    col2_1.appendChild(editbtn);
    row2_2.appendChild(col2_1);
    //削除ボタン
    var col2_2 = document.createElement("div");
    col2_2.setAttribute("class", "col-auto")
    var deletebtn = document.createElement("button");
    deletebtn.setAttribute("class", "btn btn-sm btn-outline-danger");
    deletebtn.setAttribute("onclick","DeleteState("+MaxSid+")");
    deletebtn.textContent = "削除";
    col2_2.appendChild(deletebtn);
    row2_2.appendChild(col2_2);
    col2.appendChild(row2_2);
    row1.appendChild(col2);
    NewStateLi.appendChild(row1);

    //名前編集用
    var editrow1 = document.createElement("div");
    editrow1.setAttribute("class", "row align-items-center justify-content-between")
    editrow1.setAttribute("id", "editstate-" + MaxSid);
    editrow1.style.display = "none";
    //左部分(ハンバーガーアイコン,State名)
    var editcol1 = document.createElement("div");
    editcol1.setAttribute("class", "col-auto");
    var editrow2_1 = document.createElement("div");
    editrow2_1.setAttribute("class", "row align-items-center");
    //ハンバーガーアイコン
    var editcol1_1 = document.createElement("div");
    editcol1_1.setAttribute("class", "col-auto slide-btn mx-2")
    for (var i = 0; i < 3; i++) {
        var hamburger_span = document.createElement("span");
        hamburger_span.setAttribute("class", "hamburger-bar");
        editcol1_1.appendChild(hamburger_span);
    }
    editrow2_1.appendChild(editcol1_1);
    //state名用textbox
    var editcol1_2 = document.createElement("div");
    editcol1_2.setAttribute("class", "col-auto");
    var changeinput = document.createElement("input");
    changeinput.setAttribute("type", "text");
    changeinput.setAttribute("id", "changename-" + MaxSid);
    editcol1_2.appendChild(changeinput);
    editrow2_1.appendChild(editcol1_2);
    editcol1.appendChild(editrow2_1);
    editrow1.appendChild(editcol1);
    //右部分(変更,キャンセルボタン)
    var editcol2 = document.createElement("div")
    editcol2.setAttribute("class", "col-auto");
    var editrow2_2 = document.createElement("div");
    editrow2_2.setAttribute("class", "row align-items-center");
    //変更ボタン
    var editcol2_1 = document.createElement("div");
    editcol2_1.setAttribute("class", "col-auto");
    var changebtn = document.createElement("button");
    changebtn.setAttribute("class", "btn btn-sm btn-outline-primary");
    changebtn.setAttribute("onclick", "EditStateName(" + MaxSid + ")");
    changebtn.textContent = "変更";
    editcol2_1.appendChild(changebtn);
    editrow2_2.appendChild(editcol2_1);
    //キャンセルボタン
    var editcol2_2 = document.createElement("div");
    editcol2_2.setAttribute("class", "col-auto")
    var deletebtn = document.createElement("button");
    deletebtn.setAttribute("class", "btn btn-sm btn-outline-secondary");
    deletebtn.setAttribute("onclick", "ChangeEditMode(false," + MaxSid + ")");
    deletebtn.textContent = "キャンセル";
    editcol2_2.appendChild(deletebtn);
    editrow2_2.appendChild(editcol2_2);
    editcol2.appendChild(editrow2_2);
    editrow1.appendChild(editcol2);
    NewStateLi.appendChild(editrow1);
    var AddStateLi = document.getElementById("AddState_li");
    AddStateLi.before(NewStateLi);

}
function DeleteState(id) {
    var DeleteLi = document.getElementById("state-" + id);
    DeleteLi.remove();
}
function ChangeEditMode(mode,id) {
    var dispLi = document.getElementById("dispstate-" + id);
    var editLi = document.getElementById("editstate-" + id);
    if (mode) {
        var sname = document.getElementById("sname-" + id);
        var changename = document.getElementById("changename-" + id);
        changename.value = sname.textContent;
        editLi.style.display = "flex";
        dispLi.style.display = "none";
    } else {
        editLi.style.display = "none";
        dispLi.style.display = "flex";
    }
}
function EditStateName(id) {
    var sname = document.getElementById("sname-" + id);
    var changename = document.getElementById("changename-" + id);
    sname.textContent = changename.value;
    ChangeEditMode(false, id);
}
function SubmitStateChange() {
    var States = document.querySelectorAll("#state-list .state-item");
    var sid_string = "";
    States.forEach((state,index) => {
        var sid = state.dataset.sid;
        sid_string = sid_string + (sid + ",");
        var statepara = state.querySelector(".state-name");
        var stateNameInput = document.createElement("input");
        stateNameInput.setAttribute("type", "hidden");
        stateNameInput.setAttribute("name", "state-" + index);
        stateNameInput.setAttribute("value", statepara.textContent);
        document.forms[0].appendChild(stateNameInput);
    });
    console.log(sid_string);
    var sidListInput = document.createElement("input");
    sidListInput.setAttribute("type", "hidden");
    sidListInput.setAttribute("name", "sid-list");
    sidListInput.setAttribute("value", sid_string);
    document.forms[0].appendChild(sidListInput);
}