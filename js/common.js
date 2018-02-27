function select(cond, callback) {
    var f=new Function('node','try{return '+cond+';}catch(e){return false;}');
    var c=new Function('node',callback);
    for (n in nodes) {
        var nod=nodes[n];
            if (f(nod)) c(nod);                   
    };                                        
};                                            

function aws_data(types, links, cond) {
    //dataClient.clearView();
    var command = "data ";
    if (types) {
        command = command + "types=" + types;
    };
    if (links) {
        command = command + "&&links=" + links;
    };
    if (cond) {
        command = command + "&&" + cond;
    }
    dataClient.command(command);
};

function aws_init(key, secret) {
    function func () {
        dataClient.command('auth key='+key+'&&secret='+secret)['requestCompleted(const QString&)'].disconnect(func);
    };
    dataClient.command('load aws')['requestCompleted(const QString&)'].connect(func);
};
