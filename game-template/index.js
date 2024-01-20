window.gameJson = {};

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

function getJSON(url, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'json';
    xhr.onload = function() {
      var status = xhr.status;
      if (status === 200) {
        var scene = xhr.response;
        window.gameJson[scene.id] = scene;
        callback(null, scene.id);
      } else {
        console.error("Could not download " + url);
        callback(status, null);
      }
    };
    xhr.send();
};

var progress0 = 0;
var loading_scenes = []
var loaded_scenes = []
function load_scene(sceneid) {
    loading_scenes.push(sceneid)
    console.log("loading_scenes: ", loading_scenes)
    // console.log("Loading " + sceneid);
    getJSON("./scenes/" + sceneid + ".json", function(st, sceneid) {
        if (st !== null) {
            console.error("Could not download ./game.json");
            return;
        }
        progress0 += 1;
        set_main_progress(progress0);
        loaded_scenes.push(sceneid);
        console.log("loaded_scenes: ", loaded_scenes);
        var do_loading_next_scenes = 0;
        for (var sceneid in window.gameJson) {
            // console.log("sceneid: ", sceneid)
            for (var nextid in window.gameJson[sceneid]["next"]) {
                var nextsceneid = window.gameJson[sceneid]["next"][nextid]["id"];
                // console.log("nextsceneid: ", nextsceneid)
                // console.log("window.gameJson[nextsceneid]: ", window.gameJson[nextsceneid])
                if (loading_scenes.indexOf(nextsceneid) === -1) {
                    do_loading_next_scenes ++;
                    load_scene(nextsceneid);
                }
            }
        }
        console.log("do_loading_next_scenes", do_loading_next_scenes)
        if (do_loading_next_scenes == 0 && loaded_scenes.length == loading_scenes.length) {
            console.log("All data scenes loaded");
            // TODO loading images (maybe first 10 and then loading in background)
            progress0 = 100;
            set_main_progress(progress0);
            hide_main_progress();
            show_main_play_btn();
        }
    })
}

function loading_resources() {
    progress0 = 0;
    console.warn("Loading resources...");
    load_scene("_start");
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
    var scene = window.gameJson[sceneid];
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
    var startid = '_start';
    next_scene(startid);
    hide_screen_loading();
}