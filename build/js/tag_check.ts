function tag_radiolabel_off(tag_id:string, IsChecked:boolean) {
    const tagbutton = <HTMLInputElement>document.getElementById(tag_id);
    if (IsChecked) {
        tagbutton.checked = false;
        tagbutton.onchange();
     }
}