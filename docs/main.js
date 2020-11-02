
let pages = [
   "Home",
	"Scripting Introduction",
	"Events",
	"Components"
]

let currentPage = pages[0];

function getSecondPart(str) {
    return str.split('#')[1];
}

var checkExist = setInterval(function() {
   if ($('.language-cpp').length) {
      let pres = document.getElementsByClassName('language-cpp')
      for (let ii = 0; ii < pres.length; ii++) {
          hljs.highlightBlock(pres[ii]);
      }

      clearInterval(checkExist);
   }
}, 100); // check every 100ms

window.onhashchange = function() {
	$("#" + currentPage).hide();
	currentPage = getSecondPart(window.location.href);
	if (!currentPage) {
		currentPage = pages[0].replace(" ", "_").toLowerCase();
	}
	$("#" + currentPage).show();
}

$(document).ready(function() {
   currentPage = getSecondPart(window.location.href);
   if (!currentPage) {
      currentPage = pages[0].replace(" ", "_").toLowerCase();
   }

   for (let i = 0; i < pages.length; i++) {
      let md;
      md = window.markdownit({html: true}).use(window.markdownitFootnote);

		let pageID = pages[i].replace(" ", "_").toLowerCase();

      // Load the Markdown file with jQuery.
      $.ajax({
        url: "Pages/" + pages[i] + ".md",
        dataType: "text",
        contentType: "text/plain",
        success: function(markdown){
          // Convert the Markdown to HTML.
          let html;
          html = md.render(markdown);
          // Print the HTML to #content using jQuery.
          let style = "";
          if (pageID != currentPage) {
             style = "\"display:none;\""
          }

          $("#content").append("<div style=" + style + " class=\"page\" id=\"" + pageID + "\">" + html + "</div>");
        }
      });

      $("#menu").append("<p onclick=\"LinkEvent('" + pageID + "')\"><a href=\"#" + pageID + "\">" + pages[i] + "</a></p>");
   }
});


function LinkEvent(element) {
   $("#" + currentPage).hide();
   currentPage = element;
   $("#" + currentPage).show();
}
