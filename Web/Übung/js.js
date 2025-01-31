function btich(){
    var table = document.createElement("table");
    for( var i = 0; i<= 12; i++){
        var row = document.createElement("tr");
        for(var j = 0; j<=12; j++){
            var cell = document.createElement("td");
            cell.textContent = " " + i + " " + j;
            row.appendChild(cell);
        }
        table.appendChild(row);
    }
    document.body.appendChild(table);
};