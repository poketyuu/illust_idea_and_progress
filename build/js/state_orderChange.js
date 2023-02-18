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
    NewStateTxt.value = "";
    // liの中身を生成
    var NewStateLi = document.createElement("li");
    NewStateLi.setAttribute("class", "list-group-item")
    MaxSid++;
    NewStateLi.setAttribute("id", "state-" + MaxSid);
    var row1 = document.createElement("div");
    row1.setAttribute("class", "row align-items-center justify-content-between")
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
    p_state.setAttribute("class", "my-auto");
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
    editbtn.setAttribute("class", "btn btn-outline-secondary");
    editbtn.textContent = "名前を変更";
    col2_1.appendChild(editbtn);
    row2_2.appendChild(col2_1);
    //削除ボタン
    var col2_2 = document.createElement("div");
    col2_2.setAttribute("class", "col-auto")
    var deletebtn = document.createElement("button");
    deletebtn.setAttribute("class", "btn btn-outline-danger");
    deletebtn.setAttribute("onclick","DeleteState("+MaxSid+")");
    deletebtn.textContent = "削除";
    col2_2.appendChild(deletebtn);
    row2_2.appendChild(col2_2);
    col2.appendChild(row2_2);
    row1.appendChild(col2);
    NewStateLi.appendChild(row1);
    var AddStateLi = document.getElementById("AddState_li");
    AddStateLi.before(NewStateLi);

}
function DeleteState(id) {
    var DeleteLi = document.getElementById("state-" + id);
    DeleteLi.remove();
}
function EditStateName(id) {
    
}
function SubmitStateChange() {
    
}