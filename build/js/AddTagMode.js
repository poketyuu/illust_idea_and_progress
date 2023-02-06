document.getElementById("AddTag_form").style.display = "none";
function AddTagMode(mode) {
    var AddTagForm = document.getElementById("AddTag_form");
    var AddTagBtn = document.getElementById("AddTag_btn");
    if (mode) {
        AddTagBtn.style.display = "none";
        AddTagForm.style.display = "inline";
    } else {
        AddTagBtn.style.display = "inline";
        AddTagForm.style.display = "none";
    }
}