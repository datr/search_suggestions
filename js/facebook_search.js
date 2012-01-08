Drupal.behaviors.facebook_search = function (context) {
	$('.facebook_search_submit').remove();
	$('.facebook_search_form').submit(function() {
	  if (!$('.ac_results').is(':visible')) {
			return false;
	  }
	});
	$('.facebook_search').attr('size', 27).autocomplete(Drupal.settings.basePath + "js/facebook_search/query", {
    selectFirst: false,
    dataType: 'json',
    parse: function(data) {
    	var parsed = [];
    	var term = $('.facebook_search').val();
    	for(var i in data) {
    		parsed.push({
    			data: [data[i][0]],
    			value: [data[i][0]],
    			result: term
    		});
    		for(var j in data[i][1]) {
    			parsed.push({
    				data: [data[i][1][j][0],data[i][1][j][1]],
    				value: data[i][1][j][0],
    				result: term
    			});
    		}
      }
      return parsed;
    },
    formatItem: function(row, i, count, query) {
    	if (row[1] == undefined) {
    		return '<span class="facebook-search-section">' + Drupal.settings.facebook_search.ct[row[0]] + '</span>';
    	} else {
    		return row[0];
      }
    },
    cacheLength: Drupal.settings.facebook_search.cache_length,
    delay: Drupal.settings.facebook_search.delay,
    scroll: Drupal.settings.facebook_search.scroll,
    scrollHeight: Drupal.settings.facebook_search.scroll_height,
    minChars: Drupal.settings.facebook_search.min_chars
  }).result(function(event, item) {
  	if (item[1] != undefined) {
      location.href = Drupal.settings.basePath + item[1];
    }
  });
};
