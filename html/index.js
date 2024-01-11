
window.autosave_enabled = false;

function parse_page_params() {
    var get_params = window.location.search;
    if (get_params.length > 0) {
        get_params = get_params.slice(1);
    }
    get_params = get_params.split("&");
    var result = {};
    var _regex_param = new RegExp("(.*)=([^&#]*)");
    for(var i = 0; i < get_params.length; i++){
        if(get_params[i].trim() != ""){
            var param = _regex_param.exec(get_params[i].trim());
            // console.log("results: " + JSON.stringify(p));
            if (param == null) {
                result[decodeURIComponent(get_params[i].trim().replace(/\+/g, " "))] = '';
            } else {
                result[decodeURIComponent(param[1].replace(/\+/g, " "))] = decodeURIComponent(param[2].replace(/\+/g, " "));
            }
        }
    }
    // console.log(JSON.stringify(result));
    return result;
}

function switch_ui_to_tab(_this, _callback) {
    var els = document.getElementsByClassName('pipeline-editor-tab');
    var active_id = _this.id;
    for (var i = 0; i < els.length; i++) {
        var tab_content_id = els[i].getAttribute('tab_content_id');
        if (els[i].id == active_id) {
            els[i].classList.add("active");
            document.getElementById(tab_content_id).style.display = 'block';
        } else {
            els[i].classList.remove("active");
            document.getElementById(tab_content_id).style.display = 'none';
        }
    }
    if (_callback) {
        _callback();
    }
}

function ui_render_update_states() {
    const states_elements = {
        "pipeline-editor-functions-btn moving-block": PIPELINE_EDITOR_S5_STATE_MOVING_BLOCKS,
        "pipeline-editor-functions-btn remove-block": PIPELINE_EDITOR_S5_STATE_REMOVING_BLOCKS,
        "pipeline-editor-functions-btn add-block": PIPELINE_EDITOR_S5_STATE_ADDING_BLOCKS,
        "pipeline-editor-functions-btn connect-blocks": PIPELINE_EDITOR_S5_STATE_ADDING_CONNECTIONS,
        "pipeline-editor-functions-btn disconnect-blocks": PIPELINE_EDITOR_S5_STATE_REMOVING_CONNECTIONS,
    };
    for (var n in states_elements) {
        var el = document.getElementsByClassName(n)[0];
        if (render.get_editor_state() == states_elements[n]) {
            el.classList.add('pressed');
        } else {
            el.classList.remove('pressed');
        }
    }
}

function ui_render_removing_blocks(el) {
    if (render.get_editor_state() != PIPELINE_EDITOR_S5_STATE_REMOVING_BLOCKS) {
        render.change_state_to_removing_blocks();
    } else {
        render.change_state_to_moving_blocks();
    }
    ui_render_update_states();
}

function ui_render_moving_blocks(el) {
    render.change_state_to_moving_blocks();
    ui_render_update_states();
}

function ui_render_adding_blocks(el) {
    render.change_state_to_adding_blocks();
    ui_render_update_states();
}

function ui_render_connecting_blocks() {
    render.change_state_to_adding_connections();
    ui_render_update_states();
}

function ui_render_disconnect_blocks() {
    render.change_state_to_removing_connections();
    ui_render_update_states();
}

function resize_canvas() {
    // console.log(window.innerWidth);

    var canvas_cont = document.getElementById('canvas_container');

    var left_panel = 60;
    var right_panel = 120;
    var paddings = 120;
    var height_padding = 70;

    var new_width = (window.innerWidth - left_panel - right_panel - paddings) + 'px';
    canvas_cont.style['max-width'] = new_width;
    canvas_cont.style['width'] = new_width;

    var new_height = (window.innerHeight - height_padding) + 'px';
    canvas_cont.style['max-height'] = new_height;
    canvas_cont.style['height'] = new_height;
}

function input_onchangename() {
    var block_id = document.getElementById("prop_block_id").value;
    if (block_id) {
        render.pl_data_render[block_id].set_name(document.getElementById("prop_name").value);
        render.pl_data_render[block_id].set_description(document.getElementById("prop_description").value);
        render.pl_data_render[block_id].set_color(document.getElementById("prop_color").value);
        // render.prepare_data_render();
        render.update_meansures();
        render.update_pipeline_diagram();
    }
}

function render_onselectedblock(block_id) {
    if (block_id) {
        document.getElementById("prop_block_id").value = block_id;
        document.getElementById("prop_name").value = render.pl_data_render[block_id].get_name();
        document.getElementById("prop_name").removeAttribute('readonly');
        document.getElementById("prop_description").value = render.pl_data_render[block_id].get_description();
        document.getElementById("prop_description").removeAttribute('readonly');
        document.getElementById("prop_color").value = render.pl_data_render[block_id].get_color();
        document.getElementById("prop_color").removeAttribute('readonly');
    } else {
        document.getElementById("prop_block_id").value = "";
        document.getElementById("prop_name").value = "";
        document.getElementById("prop_name").setAttribute('readonly', true);
        document.getElementById("prop_description").value = "";
        document.getElementById("prop_description").setAttribute('readonly', true);
        document.getElementById("prop_color").value = "";
        document.getElementById("prop_color").setAttribute('readonly', true);
    }
}

function render_onchanged() {
    // todo send to server
}

document.addEventListener("DOMContentLoaded", function() {
    var _data = {};
    var _params = parse_page_params();
    if (_params["v"] !== undefined) {
        // view mode
        window.render = new RenderPipelineEditor('pipeline_diagram_canvas', {
            "mode-viewer": true,
        });
        render.set_data_share(_params["v"]);
        document.getElementById("container_functions").style.display = "none";
        document.getElementById("container_properties").style.display = "none";
        document.getElementById("container_editor_tabs").style.display = "none";
        document.getElementById("tab_content_ui_editor").style.top = "5px";
        document.getElementById("tab_content_ui_editor").style.height = "calc(100% - 55px)";
    } else {
        // editor mode
        var _data = localStorage.getItem('_data');
        if (_data) {
            _data = JSON.parse(_data);
        } else {
            _data = data_pl_example;
        }
        window.render = new RenderPipelineEditor('pipeline_diagram_canvas', {
            "mode-viewer": false,
        });
        render.set_data(_data);
    }

    resize_canvas();
    render.update_meansures();
    render.update_pipeline_diagram();
    render_onselectedblock(null);


    document.getElementById("prop_name").addEventListener('keyup', input_onchangename);
    document.getElementById("prop_description").addEventListener('keyup', input_onchangename);
    document.getElementById("prop_color").addEventListener('keyup', input_onchangename);

    render.onselectedblock = render_onselectedblock;
    render.onchanged = render_onchanged;
    ui_render_update_states();
});


window.addEventListener("resize", resize_canvas);
