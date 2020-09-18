
let pages = [
   "Home",
   "Building",
   "Scripting"
]

let currentPage = pages[0];

function getSecondPart(str) {
    return str.split('#')[1];
}

var checkExist = setInterval(function() {
   if ($('.language-js').length) {
      let pres = document.getElementsByClassName('language-js')
      for (let ii = 0; ii < pres.length; ii++) {
          hljs.highlightBlock(pres[ii]);
      }

      clearInterval(checkExist);
   }
   if ($('.language-cpp').length) {
      let pres = document.getElementsByClassName('language-cpp')
      for (let ii = 0; ii < pres.length; ii++) {
          hljs.highlightBlock(pres[ii]);
      }

      clearInterval(checkExist);
   }
}, 100); // check every 100ms

$(document).ready(function() {
   currentPage = getSecondPart(window.location.href);
   if (!currentPage) {
      currentPage = pages[0];
   }

   for (let i = 0; i < pages.length; i++) {
      let md;
      md = window.markdownit({html: true}).use(window.markdownitFootnote);
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
          if (pages[i] != currentPage) {
             style = "\"display:none;\""
          }

          $("#content").append("<div style=" + style + " class=\"page\" id=\"" + pages[i] + "\">" + html + "</div>");
        }
      });

      $("#menu").append("<p onclick=\"LinkEvent('" + pages[i] + "')\"><a href=\"#" + pages[i] + "\">" + pages[i] + "</a></p>");
   }

});


function LinkEvent(element) {
   $("#" + currentPage).hide();
   currentPage = element;
   $("#" + currentPage).show();
}
