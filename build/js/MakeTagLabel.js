var tag_area = document.getElementById("tag-area");
var TagDataList = document.getElementById("Alltags");
var AddTagMap = new Map();
var tagnum = 1;
function MakeTagLabel() {
    var AddTagTxt = document.getElementById("tag-box");
    var newtag = AddTagTxt.value;
    for (let tag of AddTagMap.values()) {
        if (newtag == tag) {
            alert("このタグは既に追加されています!");
            AddTagTxt.value = "";
            return;
        }
    }
    AddTagMap.set(tagnum, newtag);
    AddTagTxt.value = "";
    var tagcol = document.createElement("div");
    tagcol.setAttribute("class", "col-auto");
    tagcol.setAttribute("id", "tag-" + tagnum);
    var newtagEle = document.createElement("p");
    newtagEle.setAttribute("class", "tag-style");
    newtagEle.setAttribute("ontouchstart", "");
    newtagEle.setAttribute("onclick", "DeleteTagLabel("+tagnum+")");
    newtagEle.textContent = ("#"+newtag);
    tagcol.appendChild(newtagEle);
    tag_area.appendChild(tagcol);
    tagnum++;
    EditSuggestTags(true, newtag);
}
function EditSuggestTags(mode, tag) {
    const suggestions = TagDataList.children;
    for (var i = 0; i < suggestions.length; i++){
        if (suggestions[i].tagName != "OPTION") continue;
        if (suggestions[i].getAttribute("value") == tag) {
            if (mode) {
                suggestions[i].setAttribute("disabled", true);
            } else {
                suggestions[i].removeAttribute("disabled");
            }
        }
    }
}
function DeleteTagLabel(id) {
    var deleteTagtext = document.getElementById("tag-" + id);
    EditSuggestTags(false, AddTagMap.get(id));
    AddTagMap.delete(id);
    deleteTagtext.remove();
}
function addTagParam() {
    var tagnum = AddTagMap.size;
    var number = 0;
    var tagnumValue = document.createElement("input");
    tagnumValue.setAttribute("type", "hidden");
    tagnumValue.setAttribute("name", "tagnum");
    tagnumValue.setAttribute("value", tagnum);
    document.forms[0].appendChild(tagnumValue);
    for (let tagname of AddTagMap.values()) {
        var tagsValue = document.createElement("input");
        tagsValue.setAttribute("type", "hidden");
        tagsValue.setAttribute("name", "tag-" + number);
        tagsValue.setAttribute("value", tagname);
        document.forms[0].appendChild(tagsValue);
        number++;
    }
}