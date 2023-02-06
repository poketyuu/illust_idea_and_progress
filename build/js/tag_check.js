function tag_radiolabel_off(tag_id, IsChecked) {
    var tagbutton = document.getElementById(tag_id);
    if (IsChecked) {
        tagbutton.checked = false;
        tagbutton.onchange();
    }
}
