
var btn1= document.querySelector('.getdata');
var btn2= document.querySelector('.export');


function fetshdata() {
    fetch("http://192.168.1.109:5555/getdatalog"
    ).then(res=>{
        res.json().then(data=>
            {
                 if (data.length >0){
                    var temp ="";
                    data.forEach(element => {
                        temp += "<tr>";
                        temp += "<td>"+element.ID+"</td>";
                        temp += "<td>"+element.nom+"</td>";
                        temp += "<td>"+element.prenom+"</td>";
                        temp += "<td>"+element.time_in+"</td></tr>";
                        
                    })
                    document.getElementById('data').innerHTML = temp;
                } 
        })
    })
}
function deleteAPI() {
    fetch("http://192.168.1.109:5555/deleteAPI"
).then(res=>{
    res.json().then(data=>
        {
            window.location.reload(false)

    })
})

    
}

function downloadCSV(csv, filename) {
    var csvFile;
    var downloadLink;

    // CSV file
    csvFile = new Blob([csv], {type: "text/csv"});

    // Download link
    downloadLink = document.createElement("a");

    // File name
    downloadLink.download = filename;

    // Create a link to the file
    downloadLink.href = window.URL.createObjectURL(csvFile);

    // Hide download link
    downloadLink.style.display = "none";

    // Add the link to DOM
    document.body.appendChild(downloadLink);

    // Click download link
    downloadLink.click();
}
function exportTableToCSV(filename) {
    var csv = [];
    var rows = document.querySelectorAll("table tr");
    
    for (var i = 0; i < rows.length; i++) {
        var row = [], cols = rows[i].querySelectorAll("td, th");
        
        for (var j = 0; j < cols.length; j++) 
            row.push(cols[j].innerText);
        
        csv.push(row.join(","));        
    }

    // Download CSV file
    downloadCSV(csv.join("\n"), filename);

     deleteAPI();


}

btn1.addEventListener("click",fetshdata)
