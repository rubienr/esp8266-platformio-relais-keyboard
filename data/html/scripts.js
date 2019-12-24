var settings = {};

/* settings = {
     "0": {
         "0": [0, 1, 2, 3, 0, 0, 0, 0],
         "1": [0, 3, 0, 0, 0, 0, 0, 0]
     },
     "1": {
         "0": [0, 1, 0, 0, 0, 0, 0, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "2": {
         "0": [0, 0, 1, 0, 0, 0, 0, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "3": {
         "0": [0, 0, 0, 1, 0, 0, 0, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "4": {
         "0": [0, 0, 0, 0, 1, 0, 0, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "5": {
         "0": [0, 0, 0, 0, 0, 1, 0, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "6": {
         "0": [0, 0, 0, 0, 0, 0, 1, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "7": {
         "0": [0, 0, 0, 0, 0, 0, 0, 1],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "8": {
         "0": [0, 0, 0, 0, 0, 0, 0, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "9": {
         "0": [0, 0, 0, 0, 0, 0, 0, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "10": {
         "0": [0, 0, 0, 0, 0, 0, 0, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "11": {
         "0": [0, 0, 0, 0, 0, 0, 0, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     },
     "12": {
         "0": [0, 0, 0, 0, 0, 0, 0, 0],
         "1": [0, 0, 0, 0, 0, 0, 0, 0]
     }
 };*/

relay_symbols = {
    toggle: "♺",
    on: "☑",
    off: "☒",
    untouched: "☐"
};

key_event_symbols = {
    pressed: "↓",
    released: "↑",
};

event_type_dict = {};
event_type_dict[0] = key_event_symbols.pressed;
event_type_dict[1] = key_event_symbols.released;

relay_actuation_dict = {};
relay_actuation_dict[0] = {id: 0, symbol: relay_symbols.off, alt: "deactivate"};
relay_actuation_dict[1] = {id: 1, symbol: relay_symbols.on, alt: "activate"};
relay_actuation_dict[2] = {id: 2, symbol: relay_symbols.toggle, alt: "toggle"};
relay_actuation_dict[3] = {id: 3, symbol: relay_symbols.untouched, alt: "leave untouched"};


function createLegend(id) {
    var r = relay_symbols;
    var k = key_event_symbols;

    var legend =
        r.on + " ... On <br />" +
        r.off + " ... Off <br />" +
        r.toggle + " ... Toggle <br />" +
        r.untouched + " ... Unchanged <br />" +
        "<br />" +
        k.pressed + " ... Button Pressed <br />" +
        k.released + " ... Button Released <br />";

    var root = document.getElementById(id);
    root.innerHTML = legend;
}

function forEachRelaySetting(callback) {
    var path = "";
    for (var key_code in settings) {
        path += key_code;
        var key_event_types = settings[key_code];

        for (var key_event_type_id in key_event_types) {
            path += "." + key_event_type_id;
            var relay_states = key_event_types[key_event_type_id];

            for (var relay_id in relay_states) {
                path += "." + relay_id;
                var relay_state = relay_states[relay_id]

                callback(path + "." + relay_state)

                path = path.slice(0, path.lastIndexOf("."));
            }
            path = path.slice(0, path.lastIndexOf("."));
        }
        path = "";
    }
}

function loadDataFromJson() {
    function setRadiobuttonEnabled(id) {
        var r = document.getElementById(id);
        r.checked = true;
    }

    forEachRelaySetting(setRadiobuttonEnabled);
}

function setColumn(row, column, event_type, value) {
    var row_class = "";
    if (row != -1)
        row_class = " key_code" + row;

    var col_class = "";
    if (column != -1)
        col_class = " relay_id" + column;

    var event_class = "";
    if (event_type != -1)
        event_class = " event_type" + event_type;

    var classes = row_class + col_class + " " + event_class + " value" + value;
    console.log("classes: " + classes);
    var buttons = document.getElementsByClassName(classes);

    for (var button_idx in buttons) {
        var button = buttons[button_idx]
        button.checked = true;
    }
}

function addSaveButton(node, text, alternative_text) {
    var button = node.appendChild(document.createElement("button"));
    button.setAttribute("type", "button");
    button.setAttribute("onClick", "store()");
    button.innerHTML = text;
    button.setAttribute("alt", alternative_text);
}

function addShortcutButton(node, text, row, column, event_type, value, alternative_text) {
    var button = node.appendChild(document.createElement("button"));
    button.setAttribute("type", "button");
    button.setAttribute("onClick", "setColumn(" +
        row + "," +
        column + "," +
        event_type + "," +
        value + ")");
    button.innerHTML = text;
    button.setAttribute("alt", alternative_text);
}

function createNodesFromJson(id) {
    var root = document.getElementById(id);

    var table = root.appendChild(document.createElement("table"));
    table.setAttribute("border", "1");

    { // table header
        var table_header = table.appendChild(document.createElement("thead"));
        var thd = table_header.appendChild(document.createElement("td"));
        thd.innerHTML = "Key #";
        thd = table_header.appendChild(document.createElement("td"));
        thd.innerHTML = "Event";

        for (var idx in settings[0][0]) {
            thd = table_header.appendChild(document.createElement("td"));
            thd.innerHTML = "Relay " + idx;
        }

        // add save button
        thd = table_header.appendChild(document.createElement("td"));
        addSaveButton(thd, "save", "save settings");
    }

    var table_body = table.appendChild(document.createElement("tbody"));
    table_body.setAttribute("align", "center");

    { // column shortcuts
        var shortcuts = table_body.appendChild(document.createElement("tr"));
        var ths = shortcuts.appendChild(document.createElement("td"));
        ths.innerHTML = "";
        ths.setAttribute("colspan", "2");

        for (var id in settings[0][0]) {
            ths = shortcuts.appendChild(document.createElement("td"));
            for (var idx in relay_actuation_dict) {
                var attrs = relay_actuation_dict[idx]
                addShortcutButton(ths, attrs.symbol, -1, id, -1, attrs.id, attrs.alt);
            }
        }

        // table shortcuts
        ths = shortcuts.appendChild(document.createElement("td"));
        for (var idx in relay_actuation_dict) {
            var attrs = relay_actuation_dict[idx]
            addShortcutButton(ths, attrs.symbol, -1, -1, -1, attrs.id, attrs.alt);
        }
    }

    var path = "";
    for (var key_code in settings) {
        path += key_code;
        var key_event_types = settings[key_code];
        var class_keycode = "key_code" + key_code;

        var table_row = table_body.appendChild(document.createElement("tr"));

        table_row.setAttribute("id", path);
        var key_name = table_row.appendChild(document.createElement("td"));
        key_name.setAttribute("rowspan", "2");
        key_name.innerHTML = /*"Key " +*/ key_code;

        var key_event_type_count = 0;
        for (var key_event_type_id in key_event_types) {
            path += "." + key_event_type_id;
            var relay_states = key_event_types[key_event_type_id];
            var class_event_type = "event_type" + key_event_type_id;

            key_event_type_count += 1;
            if (key_event_type_count >= 2) { // for rowspan
                table_row = table_body.appendChild(document.createElement("tr"));
            }

            var state = table_row.appendChild(document.createElement("td"));
            state.innerHTML = event_type_dict[key_event_type_id];

            for (var relay_id in relay_states) {
                path += "." + relay_id;
                var class_relay_id = "relay_id" + relay_id;

                var relay = table_row.appendChild(document.createElement("td"))
                relay.setAttribute("id", path);

                // create radio buttons
                for (var json_action_id in relay_actuation_dict) {
                    var input_attrs = relay_actuation_dict[json_action_id];
                    relay.appendChild(document.createTextNode(input_attrs.symbol));
                    var input = relay.appendChild(document.createElement("input"));
                    input.setAttribute("type", "radio");
                    input.setAttribute("alt", input_attrs.alt);
                    input.setAttribute("name", path);
                    input.setAttribute("id", path + "." + input_attrs.id);
                    input.setAttribute("value", /*path + "." +*/ input_attrs.id);
                    input.setAttribute("class", class_keycode + " " + class_event_type + " " + class_relay_id + " value" + input_attrs.id);
                }
                path = path.slice(0, path.lastIndexOf("."));
            }

            { // add row shortcut buttons
                var shortcuts = table_row.appendChild(document.createElement("td"));

                for (var idx in relay_actuation_dict) {
                    var attrs = relay_actuation_dict[idx]
                    addShortcutButton(shortcuts, attrs.symbol, key_code, -1, key_event_type_id, attrs.id, attrs.alt);
                }
            }

            path = path.slice(0, path.lastIndexOf("."));
        }
        path = "";
    }
}

function writeDataToJson() {
    function write_to_json(id) {
        id = id.slice(0, id.lastIndexOf("."));
        var relay_setting = document.querySelector("input[name='" + id + "']:checked");

        var key = id.slice(0, id.indexOf("."));
        id = id.slice(id.indexOf(".") + 1);

        var event = id.slice(0, id.indexOf("."));
        id = id.slice(id.indexOf(".") + 1);

        var relay = id;
        var value = relay_setting.value;

        console.log("settings[" + key + "][" + event + "][" + relay + "]=" + value);
        settings[key][event][relay] = parseInt(value);
    }

    forEachRelaySetting(write_to_json);
}

function store() {
    writeDataToJson();
    // send to µC
}


function loadJSON(url, callback) {

    var xobj = new XMLHttpRequest();
    xobj.overrideMimeType("application/json");
    xobj.open('GET', url, true); // Replace 'my_data' with the path to your file
    xobj.onreadystatechange = function () {
        if (xobj.readyState == 4 && xobj.status == "200") {
            // Required use of an anonymous callback as .open will NOT return a value but simply returns undefined in asynchronous mode
            callback(xobj.responseText);
        }
    };
    xobj.send(null);
}
