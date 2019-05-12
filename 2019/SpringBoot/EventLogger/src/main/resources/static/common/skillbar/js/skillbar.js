// Globals
var skillbar_cnt = 0;
var skillbar_array = new Array();

// Add
function addSkillbar(listId, skillbarId, showCloseBtn) {
	var tag_skillbar =
	// <!-- Chart Div Begin -->
	'<div class="col-11 align-self-center bg-light" id="' + skillbarId + '">' +
		// <!-- Chart top blank -->
		'<div class="row p-1 bg-dark"></div>' +
		// <!-- Loading spinner -->
		'<div class="row align-items-center justify-content-center p-5" id="div_spinner">' +
			'<div class="col-11 text-center align-self-center">' +
				'<div class="spinner-border text-primary" role="status">' +
					'<span class="sr-only" id="span_loading">통계를 불러오는중</span>' +
				'</div>' +
			'</div>' +
		'</div>' +
		// <!-- Chart Datas -->
		'<div class="d-none row align-items-center justify-content-between" id="div_chart">' +
			'<div class="col-6 p-1 text-left mt-1 ml-3"><b id="b_target_name"></b></div>' +
			(showCloseBtn == true ? '<div class="col-2 p-1 text-right"><button class="btn btn-light" onClick="closeSkillbar(\'' + skillbarId + '\')"><span class="fas fa-times"></span></button></div>' : '') +
			'<div class="col-12">' +
				'<div class="skillbar clearfix" data-percent="0%" id="div_income_gauge">' +
					'<div class="skillbar-title"><span id="span_income">수입</span></div>' +
					'<div class="skillbar-bar"></div>' +
					'<div class="skill-bar-percent"><span id="span_income_price">0</span>원</div>' +
				'</div>' +
				'<div class="skillbar clearfix" data-percent="0%" id="div_outgo_gauge">' +
					'<div class="skillbar-title"><span id="span_outgo">지출</span></div>' +
					'<div class="skillbar-bar"></div>' +
					'<div class="skill-bar-percent"><span id="span_outgo_price">0</span>원</div>' +
				'</div>' +
				'<div><span id="span_statistic_result_msg">수입이 지출보다 <b>0원</b> 많아/적어요!</span></div>' +
			'</div>' +
			'<div class="col-12 p-1"></div>' +
		'</div>' +
	'</div>';
	// <!-- Chart Div End -->

	$(listId).append(tag_skillbar).hide().fadeIn(500);
}

// Update Data
function updateSkillbarData(skillbarId, ajaxUrl) {
	var forNamedSkillbar = false;

	if (skillbarId == null) {
		forNamedSkillbar = true;
	}

	if (!!ajaxUrl) {
		var method = 'GET';
		var headers = { userJwt : getUserJwtFromDevice() };
		var requestParams = null;

		// AJAX call
		$.ajax({
			method : method,
			url : ajaxUrl,
			headers: headers,
			type : 'json',
			contentType : 'application/json',
			data : requestParams
		}).always(function(data_jqXHR, textStatus, jqXHR_errorThrown) {
			// ...
		}).done(function(data, textStatus, jqXHR) {
			if (data._resultCode == '00000') { // 통계계산 성공 및 수신
				var statisticsLen = data.statisticsList.length;
				
				if (statisticsLen == 0) {
					alert('통계 데이터가 없습니다.');
					return;
				}

				for (var i = 0; i < statisticsLen; ++i) {
					// 스킬바 추가
					var skillbarTag = null;

					if (forNamedSkillbar == true) { // '경조사 통계' 항목이 아닌 '검색'으로 추가하는 경우
						skillbarId = 'named_chart_' + (skillbar_cnt++);
						addSkillbar('#div_named_chart_rows', skillbarId, true);
						skillbar_array.push(skillbarId);
						skillbarId = '#' + skillbarId;
					}

					skillbarTag = $(skillbarId);

					// 통계 파싱
					var statistics = data.statisticsList[i];

					if (statistics == null) {
						continue;
					}

					var tgtName = statistics.tgtName;
					var totalIncome = statistics.totalIncome;
					var totalOutgo = statistics.totalOutgo;
					var deltaAmount = Math.abs(totalIncome - totalOutgo);
					var resultMsg = statistics.resultMsg;
					
					if (tgtName == null) {
						tgtName = '<h4>경조사 통계</h4>';
					}

					if (resultMsg == null) {
						if (totalIncome < totalOutgo) {
							resultMsg = '지출이 수입보다 <b>' + numberWithCommas(deltaAmount) + '원</b> 많아요...';
						}
						else if (totalOutgo < totalIncome) {
							resultMsg = '수입이 지출보다 <b>' + numberWithCommas(deltaAmount) + '원</b> 많아요!';
						}
						else {
							resultMsg = '지출과 수입이 같아요.'
						}
					}

					// 파싱결과 스킬바에 적용
					skillbarTag.find('#b_target_name').html(tgtName);
					skillbarTag.find('#span_income_price').html(numberWithCommas(totalIncome));
					skillbarTag.find('#span_outgo_price').html(numberWithCommas(totalOutgo));
					skillbarTag.find('#span_statistic_result_msg').html(resultMsg);

					var totalAmount = totalIncome + totalOutgo;
					var incomePercent = parseInt(((totalIncome * 100.0) / totalAmount));
					var outgoPercent = (100 - incomePercent);
					var incomeColor = '#5bc0de';
					var outgoColor = '#f0ad4e';

					if (incomePercent >= 75) { // 수입이 75% 이상
						incomeColor = '#419bf4';
					}
					else if (outgoPercent >= 75) { // 지출이 75% 이상
						outgoColor = '#ed5a47';
					}

					var div_income_gauge = skillbarTag.find('#div_income_gauge');
					div_income_gauge.find('.skillbar-title').css('background-color', incomeColor);
					div_income_gauge.find('.skillbar-bar').css('background-color', incomeColor);
					
					var div_outgo_gauge = skillbarTag.find('#div_outgo_gauge');
					div_outgo_gauge.find('.skillbar-title').css('background-color', outgoColor);
					div_outgo_gauge.find('.skillbar-bar').css('background-color', outgoColor);

					div_income_gauge.attr('data-percent', Math.max(incomePercent, 5) + '%');
					div_outgo_gauge.attr('data-percent', Math.max(outgoPercent, 5) + '%');
					
					// 스킬바 보이기
					toggleSkillbarSpinner(skillbarId, false);
				}
			}
			else { // 통계계산 실패
				alert(data._resultMsg);
			}
		}).fail(function(jqXHR, textStatus, errorThrown) {
			alert('통계 획득에 실패했습니다. (' + textStatus + ')');
		});
	}
}

// Toggle spinner
function toggleSkillbarSpinner(skillbarId, isSpinnerOn) {
	if (isSpinnerOn == true) {
		// Show Spinner, Hide Chart
		$(skillbarId).find('#div_chart').addClass('d-none');
		$(skillbarId).find('#div_spinner').removeClass('d-none');
	}
	else {
		// Hide Spinner, Show Chart
		$(skillbarId).find('#div_spinner').fadeOut(1000, function(){
			$(this).addClass('d-none');
			$(skillbarId).find('#div_chart').removeClass('d-none');

			// Animate
			$(skillbarId).find('.skillbar').each(function(){
				$(this).find('.skillbar-bar').animate({
					width:$(this).attr('data-percent')
				}, 1500);
			});
		});
	}
}

// Delete
function removeSkillbar(skillbarId, fadeOutMillis) {
	if (!fadeOutMillis || fadeOutMillis <= 0) {
		$(skillbarId).remove();
	}
	else {
		$(skillbarId).fadeOut(fadeOutMillis);
		setTimeout(function(){
			$(skillbarId).remove();
		}, fadeOutMillis + 10);
	}
}

// Close Button
function closeSkillbar(skillbarId) {
	if (skillbarId.charAt(0) != '#') {
		skillbarId = '#' + skillbarId;
	}

	removeSkillbar(skillbarId, 300);
}