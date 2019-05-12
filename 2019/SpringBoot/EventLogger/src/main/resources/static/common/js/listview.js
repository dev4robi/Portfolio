var __contador = 0;

// 리스트에 추가
function add_to_list(typeNum, eventSeq, title, memo, date, amount) {
	var typeName = "DefaultEventTypeName";
	var _typeNum = typeNum;

	switch (_typeNum) {
		case 0:
			typeName = "지출";
			break;
		case 1:
			typeName = "수입"
			break;
		case 2:
			typeName = "기타";
			break;
		default:
			typeName = "미정의";
			break;
	}

	var cont = '<input type="hidden" name="eventSeq" value="' + eventSeq + '"/>'
				 + '<div class="col_md_1_list"><p>' + typeName + '</p></div>'
				 + '<div class="col_md_2_list"><h4>' + title + '</h4><p>' + memo + '</p></div>'
				 + '<div class="col_md_3_list"><div class="cont_text_date"><p>'
				 + date + '</p></div><div class="cont_text_amount"><p class="t'
				 + typeNum + '">' + amount + '</p></div></div>';

	var contNum = __contador;
	var li = document.createElement('li')
	li.className = "list_t" + _typeNum + " li_num_" + contNum + " list_dsp_none";

	li.innerHTML = cont;
	document.querySelectorAll('.cont_princ_lists > ul')[0].appendChild(li);

	setTimeout(function() {
		document.querySelector('.li_num_' + contNum).style.display = "block";
	}, 100);

	setTimeout(function() {
		document.querySelector('.li_num_' + contNum).className = "list_t"
				+ _typeNum + " li_num_" + contNum + " list_dsp_true";
	}, 200);

	++__contador;
}

// 리스트에서 제거
function finish_action(listNum) {
	document.querySelector('.li_num_' + listNum).className = "list_finish_state";

	setTimeout(function() {
		del_finish();
	}, 300);
}

// 직접 사용 금지
function del_finish() {
	var li = document.querySelectorAll('.list_finish_state');
	for (var e = 0; e < li.length; e++) {
		/* li[e].style.left = "-100px"; */
		li[e].style.opacity = "0";
		li[e].style.height = "0px";
		li[e].style.margin = "0px";
	}

	setTimeout(function() {
		var li = document.querySelectorAll('.list_finish_state');
		for (var e = 0; e < li.length; e++) {
			li[e].parentNode.removeChild(li[e]);
		}
	}, 500);
}

// 직접 사용 금지
var __tcnt = 0;
function add_new() {
	if (__tcnt % 2 == 0) {
		document.querySelector('.cont_crear_new').className = "cont_crear_new cont_crear_new_active";
		document.querySelector('.cont_add_titulo_cont').className = "cont_add_titulo_cont cont_add_titulo_cont_active";
		++__tcnt;
	} else {
		document.querySelector('.cont_crear_new').className = "cont_crear_new";
		document.querySelector('.cont_add_titulo_cont').className = "cont_add_titulo_cont";
		++__tcnt;
	}
}
