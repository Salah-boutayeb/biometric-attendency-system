var Express = require('express');
var app = Express();
var http = require('http').createServer(app);
var path =require('path');
const bodyParser = require("body-parser");

var mysql = require('mysql');
var pool        = mysql.createPool({
  conLimit : 10, // default = 10
  host: "localhost",
  user: "root",
  password: "",
  database: "etudiants"
});  
const port = process.env.PORT || 5555;
http.listen(port ,()=> console.log('server is linsting on',port));

//app.use(cors())
app.use(Express.json())
app.use(Express.static('../html'))
app.get('/', function(req, res){

    res.sendFile(path.join(__dirname,'../html/att.html'));

  });
app.get('/getdatalog',(req, res) => {
        pool.getConnection( function (err, connection) {
              connection.query("SELECT * FROM etudiant_log  ", function (err, rows) {
              if (err) throw err;
              connection.release(); 
              res.json(rows);       
        });   
    });
}); 



app.post('/index',(req, res) => {
    var fingerID = req.body.FingerID;
          pool.getConnection( function (err, connection) {
          connection.query("SELECT * FROM etudiant WHERE fingerID = "+mysql.escape(fingerID), function (err, rows) {
            var data = [];
            if (err) throw err;
            data= [rows[0].ID,rows[0].nom,rows[0].prenom,rows[0].filiere,rows[0].fingerID];
            var  d={
              ID: rows[0].ID,
              nom: rows[0].nom,
              prenom: rows[0].prenom,
              time_in : new Date().toUTCString()
            }
            connection.query("INSERT INTO etudiant_log set ?",d, function (err, rows) {
              if (err) throw err;  
              connection.release(); 
              console.log(d);
              res.json(data[1]);                         
          });          
      }); 
        console.log("fingerID = "+fingerID); 
    });
});
app.get('/deleteAPI',(req,res) => {
  pool.getConnection( function (err, connection) {
    connection.query("DELETE FROM etudiant_log", function (err, rows) {
      
      if (err) throw err;
     console.log("data deleted");
     res.json(rows);  
    });  
  }); 
});
