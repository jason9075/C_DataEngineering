/* gobal varible */
var jsonArray = new Array(); // get json and store into array

function get_jsonData(page) {
	var options = document.getElementById('options').value;
	// var argument = options.split(" ").join('+');		//for sorting
	var argument = "-r+" + options;
	argument += "+-p+" + page;
	$.ajax({
	  url: "cgi-bin/sort.cgi?" + argument,
	  type: "GET",
	  dataType: "json",
	  success: function(JData) {
			jsonArray = []; //clear
			var dataLength = 0;
			$.each(JData.records, function() {
				jsonArray.push(JData.records[dataLength]);
			  	dataLength++;
			}); 
			dataDisplay(dataLength);
			pagination(JData.recordsNum, page);
			document.getElementById("dataLength").innerHTML=JData.recordsNum;
	  },

	  error: function(xhr) {
	    alert("ERROR!!!" + xhr.status);
	  }
	});
}

function tableTSort(option) {
	if (document.getElementById('options').value == "-r " + option)
		document.getElementById('options').value = "-r " + option + " -i";
	else
		document.getElementById('options').value = "-r " + option;
	get_jsonData(1);
}

function dataDisplay(dataLength) {
	var output = "";

	// output += "<div class=\"css_tr\" style=\"background:#fcf8e3\">" ;
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('id')\">ID</a></div>";
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('published')\">Published</a></div>";
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('updated')\">Updated</a></div>";
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('title')\">Title</a></div>";
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('content')\">Content</a></div>";
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('author')\">Author</a></div>";
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('keyword')\">Keyword</a></div>";
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('favoriteCount')\">Favorite Count</a></div>";
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('viewCount')\">View Count</a></div>";
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('duration')\">Duration</a></div>";
	// output += "<div class=\"css_td\"><a href=\"#\" onclick=\"tableTSort('category')\">Category</a></div>";
	// output += "</div>";
	for(var k = 1; k < dataLength ; k++) {
		if (k%2==1)
			output += "<div class=\"row\">";
		output += "<div class=\"col-xs-12 col-sm-6 col-md-6\">";
		output += "<div class=\"thumb\">  <div class=\"thumb-inbox\">" ;
		output +=  "<div class=\"thumb-image\">   <p>   <img src=\"http://i1.ytimg.com/vi/"+ jsonArray[k].id.slice(42,53) +"/hqdefault.jpg\" >   </p>   </div>";
        output += "<div class=\"thumb-details\">  <a href=\"#result\" onclick=\"popup('popUpDiv',"+k+")\">  <div class=\"thumb-details-overview\">   <p>Author : " +jsonArray[k].author+"</p>   <div class=\"thumb-review\">" +jsonArray[k].content + " </div>  <p class=\"thumbDate\" align = \"right\">" +jsonArray[k].published+"</p>  </div>  </a>  </div>";
	    output += " <div class=\"thumb-info\">   <div class=\"thumb-title\">   <div class=\"thumb-name\"> <a href=\"#result\" onclick=\"popup('popUpDiv',"+k+")\">   <h4>   "+jsonArray[k].title+"   </h4>   </a>  </div>   </div>   </div>";
		// output +=  jsonArray[k].title    ;
		// output += jsonArray[k].published  ;
		// output += "<div class=\"css_td\">" + jsonArray[k].updated    + "</div>";
		// output += "<div class=\"css_td\">" + jsonArray[k].content    + "</div>";
		// output += jsonArray[k].author  ;
		// output += "<div class=\"css_td\">" + jsonArray[k].keyword    + "</div>";
		// output += "<div class=\"css_td\">" + jsonArray[k].favoriteCount    + "</div>";
		// output += jsonArray[k].viewCount   ;
		// output +=  jsonArray[k].duration    ;
		// output +=  jsonArray[k].category   ;
		output += "</div> </div> </div>"; //for thumb-inibox thumb col
		if (k%2==0)
			output += "</div>"; // for row
	}

	$("#JSON_table").html(output);
}

function pagination(recordsNum, currentPage) {
	// var output = "Total: " + recordsNum + " <br>";
	var totalPage = parseInt(recordsNum/10)+1;
	$("#paginationUl").empty();
	for (var l = currentPage - 1 ; l >= currentPage - 6 && l > 0; l--) {
		$("#paginationUl").prepend("<li><a href=\"#result\" onclick=\"get_jsonData(" + l +")\">" + l + "</a></li>");
	}
	
	$("#paginationUl").prepend("<li><a href=\"#result\" onclick=\"get_jsonData(1)\">" + "<<" + "</a></li>");
	
	$("#paginationUl").append("<li class=\"active\"><a href=\"#result\" onclick=\"get_jsonData(" + currentPage +")\">" + currentPage + "</a></li>");				
	for (var r = currentPage + 1; r <= currentPage + 6 && r <= totalPage ; r++) {
	  $("#paginationUl").append("<li><a href=\"#result\" onclick=\"get_jsonData(" + r +")\">" + r + "</a></li>");
	}
	$("#paginationUl").append("<li><a href=\"#result\" onclick=\"get_jsonData(" + totalPage +")\">" + ">>" + "</a></li>");			
	$("#paginationUl").prepend("Total: " + recordsNum + " <br>");
}	

function toggle(div_id,videoNum) {
	var el = document.getElementById(div_id);
	if ( el.style.display == 'none' ) {	
		if (videoNum== -1)
			el.style.display = 'block';
		else{
			el.style.display = 'block';
			var url = "//www.youtube.com/embed/"+jsonArray[videoNum].id.slice(42,53);
			document.getElementById("popYoutube").src=url;
		}
	}
	else {
		if (videoNum== -1)
			el.style.display = 'none';
		else{
			el.style.display = 'none';
			document.getElementById("popYoutube").src="";
		}
	}
}
function blanket_size(popUpDivVar) {
	if (typeof window.innerWidth != 'undefined') {
		viewportheight = window.innerHeight;
	} else {
		viewportheight = document.documentElement.clientHeight;
	}
	if ((viewportheight > document.body.parentNode.scrollHeight) && (viewportheight > document.body.parentNode.clientHeight)) {
		blanket_height = viewportheight;
	} else {
		if (document.body.parentNode.clientHeight > document.body.parentNode.scrollHeight) {
			blanket_height = document.body.parentNode.clientHeight;
		} else {
			blanket_height = document.body.parentNode.scrollHeight;
		}
	}
	var blanket = document.getElementById('blanket');
	blanket.style.height = blanket_height + 'px';
	var popUpDiv = document.getElementById(popUpDivVar);
	popUpDiv_height=blanket_height/2-350;//150 is half popup's height
	popUpDiv.style.top = popUpDiv_height + 'px';
}
function window_pos(popUpDivVar) {
	if (typeof window.innerWidth != 'undefined') {
		viewportwidth = window.innerHeight;
	} else {
		viewportwidth = document.documentElement.clientHeight;
	}
	if ((viewportwidth > document.body.parentNode.scrollWidth) && (viewportwidth > document.body.parentNode.clientWidth)) {
		window_width = viewportwidth;
	} else {
		if (document.body.parentNode.clientWidth > document.body.parentNode.scrollWidth) {
			window_width = document.body.parentNode.clientWidth;
		} else {
			window_width = document.body.parentNode.scrollWidth;
		}
	}
	var popUpDiv = document.getElementById(popUpDivVar);
	// window_width=window_width/2-150;//150 is half popup's width
	window_width = window_width/2-336;
	popUpDiv.style.left = window_width + 'px';
}

function popup(windowname,videoNum) {
	blanket_size(windowname);
	window_pos(windowname);
	toggle('blanket',-1); 
	toggle(windowname,videoNum);
}


var gg= 1;
$(document).ready(function (){

  /* Every time the window is scrolled ... */
    $(window).scroll( function(){

        var bottom_of_object;
        var bottom_of_window;

        /* Check the location of each desired element */
        $('.navbar-inverse').each( function(gg){
            
            bottom_of_object = $(result).offset().top - 10 ;
            bottom_of_window = $(window).scrollTop() ;
            
            /* If the object is completely visible in the window, fade it it */
            if( bottom_of_window > bottom_of_object ){
                $(this).css({'-webkit-transform':'translateY(0)','-o-transform': 'translateY(0)','-moz-transform': 'translateY(0)'});
            }else{
                $(this).css({'-webkit-transform':'translateY(-50px)','-o-transform': 'translateY(-50px)','-moz-transform': 'translateY(-50px)'});
            }

        }); 
    });
});


