
var fetch = function(url, callback, error) {

	var request = new XMLHttpRequest();
	request.open('GET', url, true);

	request.onload = function() {
	  if (request.status >= 200 && request.status < 400) {
	    // Success!
	    var data = JSON.parse(request.responseText);
	    callback(data);

	  } else {
	    error(request)
	  }
	};

	request.onerror = function() {
	  error(this)
	};

	request.send();
}


var initConfig = function(config) {

	config.forEach(function(tile, index) {

		var target = document.getElementById("tile" + index);

		if(target) {
			if(tile.color.substr(0, 1) == "#") {
				target.style.backgroundColor = tile.color;
				tile.color = 'custom';
			}

			target.classList.add(tile.color, tile.type, "active");
			if(tile.letter) {
				target.innerHTML = "<span>" + tile.letter + "</span>";
			}
			if(tile.group) {
				var group = document.createAttribute("group");
				group.value = tile.group + '';
				target.attributes.setNamedItem(group);
				target.classList.add("group" + tile.group);
			}

			var key = document.createAttribute("key");
			key.value =index;
			target.attributes.setNamedItem(key);

 		}

	});
}


var enterElement = function(ev, el) {
	ev.preventDefault();
	ev.stopPropagation();

	if(el.classList.contains("momentary")) {
		el.classList.add("on");
		return true;
	}
}

var exitElement = function(ev, el) {
	ev.preventDefault();
	ev.stopPropagation();

	if(el.classList.contains("momentary")) {
		el.classList.remove("on");
		return;
	}

	if(el.classList.contains("group")) {

		document.querySelectorAll(".group" + el.attributes.getNamedItem("group").value).forEach(function(elm) {
			if(elm == el) {
				elm.classList.toggle("on");
			} else {
				elm.classList.remove("on");
			}
		});

		testHandle()

		return;

	}

	if(el.classList.contains("active")) {
		el.classList.toggle("on");
	}
}


setTimeout(function() {

	for(var x = 0; x < 15; x++) {

		document.getElementById("tile" + x).onmousedown=function(ev){enterElement(ev, this)}
		document.getElementById("tile" + x).onmouseup=function(ev){exitElement(ev, this)}
		document.getElementById("tile" + x).ontouchstart=function(ev){enterElement(ev, this)}
		document.getElementById("tile" + x).ontouchend=function(ev){exitElement(ev, this)}
	}

	initConfig(testConfig);

}, 250);
