function deleteFile() {
    let filename = document.getElementById("fileDeleteNew").value;
    console.log("File name:", filename);

    if (!filename) {
        alert("Enter filename");
        return;
    }

    fetch(filename, {
        method: "DELETE"
    })
    .then(response => {
        alert(`Server return: ${response.status} ${response.statusText}`);
    })
    .catch(error => alert("Error: " + error));
}