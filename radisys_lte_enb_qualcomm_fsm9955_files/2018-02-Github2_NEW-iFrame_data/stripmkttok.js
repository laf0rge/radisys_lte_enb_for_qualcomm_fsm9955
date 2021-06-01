// LM-100892: Once page process mkt_tok, remove parameter from URL
(function() {
    var afterLoad = function (fn) {
        if (window.attachEvent) {
            window.attachEvent('onload', fn);
        } else {
            window.addEventListener('load', fn);
        }
    };
    afterLoad(function () {
      // LM-123191: Remaining '&' from removal of 'mkt_tok=' in tracked links from emails
      // Parse all query parameters of location.search, remove mkt_tok, then reassemble the rest
      if (location.search && location.search.length > 1 && location.search[0] == '?') {
        var newSearch = '', stripped = false, params = location.search.substring(1).split('&');
        params.forEach(function(item) {
          if (item) {
            if (item.indexOf('mkt_tok=') === 0) {
              // Cache mkt_tok value into global variable, before stripping it from URL
              window.__mktTokVal = window.__mktTokVal || item.substring(8);
              stripped = true;
            }
            else {
              // Put together the new "search" query except the leading '?'
              newSearch += (newSearch ? '&' : '') + item;
            }
          }
        });
        if (stripped) {
          // If, and only if, mkt_tok= parameter was found and stripped
          // Replace window history entry with updated search portion of original URL 
          window.history && history.replaceState('', null, location.pathname + (newSearch ? '?' + newSearch : '') + location.hash);
        }
      }
    });
})();
