## Qt Client

Overview
--------

The main idea of that tool is to display and interact(TBD) with any hierarhical data in a suitable and comfort for you way.
Just decide which driver to use, how to display your data and which actions(TBD)
are allowed on them and you are all set. Tool provide suitable way to access collected
data via javascript and work with them. You will be able to have as many data views 
as you wish. You don't have to generate any html charts before displaying a data, just 
configure the **Grapher**, tell it where to get data, select driver
(for now, only [AWS driver](https://github.com/qfoxic/grapher-server-aws/blob/master/README.md) is available)
and play.

UI
--

##### Controls

UI is split into three sections: data view, control tabs, command line.
![ui.png](https://github.com/qfoxic/grapher-client-qt/blob/master/doc/ui.png "ui.png")
1. **Data view** - is where you interact with displayed items.

    * **Javascript configuration**
    
      When application starts it searches for a **js/VIEWNAME.js** file in a **js** directory
      and executes it. So, this feature can be used to manipulate with data items.
      **VIEWNAME** is what you specified in a **config.js**.
      You can select items by mouse and move them.
      If you move parent item all its children will be moved as well.
      Doubleclick on a view means the same as enlarge view.
 
    * **Exposed javascript structures**

        - **nodes** javascript array that contains all currently displayed items
          and exposed to every js file.
          Example, how to use **nodes** structure.
             ```javascript
                function select(cond, callback) {
                    // this function evaluate *cond* on every node from *nodes* array
                    // and executes *callback* when *cond* was evaluated to true. 
                    var f=new Function('node','try{return '+cond+';}catch(e){return false;}');
                    var c=new Function('node',callback);
                    for (n in nodes) {
                       var nod=nodes[n];
                       if (f(nod)) c(nod);                   
                    };                                        
                };                                            
             ```
                    
        - **dataClient** proxy to access backend.
          Example, how to use **dataClient** structure.
          ```javascript
             function aws_init(key, secret) {
                 function func () {
                      dataClient.command('auth key='+key+'&&secret='+secret)['requestCompleted(const QString&)'].disconnect(func);
                 };
                 // This example demostrates how to connect to a *dataClient* signal.
                 dataClient.command('load aws')['requestCompleted(const QString&)'].connect(func);
             };
             
             // This example demostrates how to connect to a backend session via *dataClient* proxy.
             function aws_data(types, links, cond) {
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
                // This call will send prepared command via session to a backend.
                dataClient.command(command);
             };
          ```
          
        
2. **Control tabs** - configurable by js/config.js tabs with a set of different tool buttons.

    - **Javascript configuration**
    
      Control tabs are configured by js/config.js, so you can add as many tabs as you wish.
      **Note!** - one grapher-server process can serve only one session from a client.
      If you wish to have more sessions you have to start more grapher-servers listening on
      different ports.
      ```javascript
        // Configuration is a js object that must be assigned to a config variable 
        config = {
            // View name - this name will be used to load according js file.  
            "aws": { 
             "host": "127.0.0.1",
             "port": 9999,
             "types": {
                // Every item in a system has it's type.
                // For now, you can configure only node color.
                "ec2": {"color": "yellow"},
                "sg": {"color": "red"},
                "region": {"color": "green"},
                "vpc": {"color": "olive"}
             }
            },
            "view2": {
             "host": "127.0.0.1",
             "port": 8888,
             "types": {
                 "elb": {"color": "blue"},
                 "ec2": {"color": "yellow"}
             }
            }
        };
      ```

    - **Tool buttons**
    
      **View section**
        - ![decr.png](https://github.com/qfoxic/grapher-client-qt/blob/master/doc/decr.png "decr.png") - makes your view smaller.
        - ![incr.png](https://github.com/qfoxic/grapher-client-qt/blob/master/doc/incr.png "incr.png") - makes your view bigger.
        - ![clear.png](https://github.com/qfoxic/grapher-client-qt/blob/master/doc/clear.png "clear.png") - clear you view and **nodes** js structure.
        - ![fit.png](https://github.com/qfoxic/grapher-client-qt/blob/master/doc/fit.png "fit.png") - resize your view according to your visible content to make it fit into viewport.

      **Connection section**      
        - ![refresh.png](https://github.com/qfoxic/grapher-client-qt/blob/master/doc/refresh.png "refresh.png") - will refresh your backend session in case you've lost connection or so.
      
3. **Command line** - a place where you interact with a backend and actually shown data.

    - **Javascript configuration**

      When application starts it loads **js/config.js** and configure UI according to 
      what is there. Then it loads **js/common.js** and put all content: variables and functions
      into global namespace. Then, if present, loads javascript files for every configured 
      view, the name of a view will be used to find according js file.
