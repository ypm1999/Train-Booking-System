var app = require('http').createServer(handler);
var io = require('socket.io')(app);
var fs = require('fs');

app.listen(15485, '0.0.0.0');

function handler(req, res) {
    fs.readFile(__dirname + '/index.html',
        function (err, data) {
            if (err) {
                res.writeHead(500);
                return res.end('Error loading index.html');
            }
            res.writeHead(200);
            res.end(data);
        });
}

io.on('connection', function (socket) {
    console.log("Client IP: ", socket.handshake.address);
    socket.on('pub', function (data) {
        console.log("Subscriber client IP: ", socket.handshake.address, 'data: ', data);
        socket.broadcast.emit('status', data);
    });
    /*socket.on('kitjs', function(data) {
     console.log("Judge event: ", socket.handshake.address, 'data: ', data);
     socket.broadcast.emit('judge', data);
     });*/
});

