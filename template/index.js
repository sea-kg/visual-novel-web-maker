window.gameJson = {};

function getJSON(url, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'json';
    xhr.onload = function() {
      var status = xhr.status;
      if (status === 200) {
        window.gameJson = xhr.response;
        callback(null);
      } else {
        callback(status);
      }
    };
    xhr.send();
};

function show_main_progress() {
    document.getElementById("main_loader").style.display = "";
}

function hide_main_progress() {
    document.getElementById("main_loader").style.display = "none";
}

function set_main_progress(new_value) {
    document.getElementById("main_loader_progress").style.width = new_value + "%";
}

function show_main_play_btn() {
    document.getElementById("main_play_btn").style.display = "";
}

function hide_main_play_btn() {
    document.getElementById("main_play_btn").style.display = "none";
}

function hide_screen_loading() {
    document.getElementById("screen_loading").style.display = "none";
}

function loading_resources() {
    console.warn("Simulation loading resources... TODO");
    getJSON("./game.json", function(st) {
        if (st !== null) {
            console.error("Could not download ./game.json");
            return;
        }
        console.log("window.gameJson: ", window.gameJson)
        for (var sceneid in window.gameJson["scenes"]) {
            var url_background = window.gameJson["scenes"][sceneid]["background"];
            console.log("url_background", url_background)
        }
        // TODO parse and
        var progress0 = 0;
        var interval0 = setInterval(function() {
            set_main_progress(progress0);
            progress0 += 10;
            if (progress0 > 100) {
                clearInterval(interval0);
                hide_main_progress();
                show_main_play_btn();
            }
        }, 100);
    })
}

document.addEventListener("DOMContentLoaded", (event) => {
    loading_resources();
});

function onclick_next_scene(el) {
    var sceneid = el.srcElement.getAttribute("next_scene_id");
    next_scene(sceneid);
}

function onclick_next_scene_btn(el) {
    var sceneid = el.getAttribute("next_scene_id");
    next_scene(sceneid);
}

function escapeHtml(html){
    var text = document.createTextNode(html);
    var p = document.createElement('p');
    p.appendChild(text);
    return p.innerHTML;
}

function next_scene(sceneid) {
    console.log("next_scene ", sceneid)
    var scene = window.gameJson["scenes"][sceneid];
    document.getElementById("scene_background_back").style["background-image"] = "url(\"" + scene["background"] + "\")";
    document.getElementById("scene_background_front").style["background-image"] = "url(\"" + scene["background"] + "\")";
    document.getElementById("scene_text").innerHTML = scene["text"];
    document.getElementById("scene_text_panel").onclick = null;
    document.getElementById("scene_background_front").onclick = null;
    document.getElementById("scene_text_panel").setAttribute("next_scene_id", "");
    document.getElementById("scene_background_front").setAttribute("next_scene_id", "");
    document.getElementById("scene_buttons_panel").style.display = "none";
    document.getElementById("scene_buttons_content").innerHTML = "";

    if (scene["next"].length == 1) {
        document.getElementById("scene_text_panel").setAttribute("next_scene_id", scene["next"][0]["id"]);
        document.getElementById("scene_background_front").setAttribute("next_scene_id", scene["next"][0]["id"]);

        document.getElementById("scene_text_panel").onclick = onclick_next_scene;
        document.getElementById("scene_background_front").onclick = onclick_next_scene;
    } else {
        document.getElementById("scene_buttons_panel").style.display = "";
        for (var i in scene["next"]) {
            var btn_text = scene["next"][i]["text"];
            var btn_id = scene["next"][i]["id"];

            document.getElementById("scene_buttons_content").innerHTML += ""
                + "<div class='scene-button-choise' "
                + " next_scene_id='" + btn_id + "' "
                + " onclick='onclick_next_scene_btn(this);'>"
                + escapeHtml(btn_text) + "</div>";
        }
        // variants
    }
}

function run_game() {
    var startid = window.gameJson["options"]["start-scene"];
    next_scene(startid);
    hide_screen_loading();
}