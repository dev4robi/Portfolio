<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport"
    content="width=device-width,initial-scale=1.0,minimum-scale=1.0,maximum-scale=1.0">
<script type="text/javascript" src="/lib/jquery-3.4.0/jquery-3.4.0.min.js"></script>
<script>
    // 테스트 스위치
    var localtest = true;

    //Statistics AJAX 호출 시
    function submitStatistics() {
        var restURL = (localtest == false ? 'http://ec2-3-17-11-139.us-east-2.compute.amazonaws.com' : 'http://localhost:8080');
        var headers = {
            userJwt : $("input[name='userJwt']").val()
        };

        restURL += '/statistics/all';

        $.ajax({
            method : 'GET',
            url : restURL,
            headers: headers,
            type : 'json',
            contentType : 'application/json',
            data : null
        }).done(function(data, textStatus, jqXHR) {
            //alert('Done! - method: ' + var_method + '\ndata: ' + JSON.stringify(data) + '\n textStatus:' + textStatus + '\n jqXHR:' + jqXHR);
            jsonData = JSON.stringify(data);
            alert(jsonData);
        });
    }
   
    // Events AJAX 호출 시
    function submit(forJwt) {
        var restURL = (localtest == false ? 'http://ec2-3-17-11-139.us-east-2.compute.amazonaws.com' : 'http://localhost:8080');
        var headers = null;
        var datas = null;
        var methods = null;

        // JWT 생성용
        if (forJwt == 'true') {
            var var_userId = $("input[name='userId']").val();

            method = 'GET';
            restURL += '/test/jwt/id=' + var_userId;
        } else {
            // 경조사 테스트용 
            var var_userJwt = $("input[name='userJwt']").val();
            var var_evtSeq = $("input[name='evtSeq']").val();
            var var_evtBoardPage = $("input[name='evtBoardPage']").val();
            var var_evtDate = $("input[name='evtDate']").val();
            var var_evtTgtUserName = $("input[name='evtTgtUserName']").val();
            var var_evtTgtUserPhoneNum = $("input[name='evtTgtUserPhoneNum']").val();
            var var_evtTypeCode = $("input[name='evtTypeCode']").val();
            var var_relationTypeCode = $("input[name='relationTypeCode']").val();
            var var_payAmount = $("input[name='payAmount']").val();
            var var_evtMemo = $("input[name='evtMemo']").val();

            // 
            method = $("select[name='method']").val();
            restURL += '/events';

            // 
            if (method == "POST" || method == "PUT") {
                datas = {
                    userJwt : var_userJwt,
                    eventDao : {
                        evtSeq : var_evtSeq,
                        evtDate : var_evtDate,
                        evtTgtUserName : var_evtTgtUserName,
                        evtTgtUserPhoneNum : var_evtTgtUserPhoneNum,
                        evtTypeCode : var_evtTypeCode,
                        relationTypeCode : var_relationTypeCode,
                        payAmount : var_payAmount,
                        evtMemo : var_evtMemo
                    }
                };
            } else if (method == "DELETE") {
                datas = {
                    userJwt : var_userJwt,
                    evtSeq : var_evtSeq,
                };
            } else { // GET
                headers = {
                    userJwt : var_userJwt
                };

                var getPayType = $("select[name='payType']").val();
                getPayType = getPayType.toLowerCase(); 

                if (getPayType == 'all')
                    restURL += ('/all');
                else if (getPayType == 'outgo')
                    restURL += ('/outgo');
                else
                    restURL += ('/income');

                restURL += ('/page/' + var_evtBoardPage);
            }

            //
            datas = JSON.stringify(datas);
        }

        if (!confirm('Call AJAX?')) {
            return;
        }

        //
        $.ajax({
            method : method,
            url : restURL,
            headers: headers,
            type : "json",
            contentType : "application/json",
            data : datas
        }).always(
                function(data_jqXHR, textStatus, jqXHR_errorThrown) {
                    //alert('Always! - method: ' + var_method + '\ndata_jqXHR: ' + data_jqXHR +
                    //      '\n textStatus:' + textStatus + '\n jqXHR_errorThrown:' + jqXHR_errorThrown);
                    $("textarea[name='sendData']").prop('readonly', false);
                    $("textarea[name='recvData']").html('');
                    $("textarea[name='sendData']").html(
                            'URL : ' + restURL + '\n\n' + datas);
                    $("textarea[name='sendData']").prop('readonly', true);
                }).done(function(data, textStatus, jqXHR) {
            //alert('Done! - method: ' + var_method + '\ndata: ' + JSON.stringify(data) + '\n textStatus:' + textStatus + '\n jqXHR:' + jqXHR);
            jsonData = JSON.stringify(data);
            $("textarea[name='recvData']").prop('readonly', false);
            $("textarea[name='recvData']").html(jsonData);
            $("textarea[name='recvData']").prop('readonly', true);

            if (data.userJwt != null) {
                $("input[name='userJwt']").val(data.userJwt);
            }
        }).fail(function(jqXHR, textStatus, errorThrown) {
            //alert('Fail! - method: ' + var_method + '\njqXHR: ' + JSON.stringify(jqXHR) + '\n textStatus:' + textStatus + '\n errorThrown:' + errorThrown);
            $("textarea[name='recvData']").prop('readonly', false);
            $("textarea[name='recvData']").html(JSON.stringify(jqXHR));
            $("textarea[name='recvData']").prop('readonly', true);
        });
    }

    //
    function hideAllInput() {
        $("#p_evtSeq").hide();
        $("#p_evtBoardPage").hide();
        $("#p_evtPayType").hide();
        $("#p_evtDate").hide();
        $("#p_evtTgtUserName").hide();
        $("#p_evtTgtUserPhoneNum").hide();
        $("#p_evtTypeCode").hide();
        $("#p_relationTypeCode").hide();
        $("#p_payAmount").hide();
        $("#p_evtMemo").hide();
    }

    //
    function getTimeStamp() {
        var d = new Date();
        var s = leadingZeros(d.getFullYear(), 4) + '-'
                + leadingZeros(d.getMonth() + 1, 2) + '-'
                + leadingZeros(d.getDate(), 2) + ' ' +

                leadingZeros(d.getHours(), 2) + ':'
                + leadingZeros(d.getMinutes(), 2) + ':'
                + leadingZeros(d.getSeconds(), 2);

        return s;
    }

    //
    function leadingZeros(n, digits) {
        var zero = '';
        n = n.toString();

        if (n.length < digits) {
            for (i = 0; i < digits - n.length; i++)
                zero += '0';
        }

        return zero + n;
    }
</script>
<style>
.textwrapper {
    border: 1px solid #999999;
    margin: 5px 0;
    padding: 3px;
}
</style>
<title>Test Admin Page</title>
</head>
<body>
    <hr>
    <p>
        JWT 생성 : <input type="email" style="width: 50%;" name="userId"
            placeholder="evtlogger@gmail.com" />
    </p>
    <p>
        <input type="button" name="submit" style="width: 150px; height: 30px;"
            value="생성" onclick="submit('true')" />
    </p>
    <p>
        <input type="button" name="submit" style="width: 150px; height: 30px;"
            value="통계" onclick="submitStatistics()" />
    </p>
    <hr>
    <p>
        JWT : <input type="text" style="width: 50%;" name="userJwt" />
    </p>
    <p>
        <select id="methodSelect" name="method">
            <option value="NONE">Select Method</option>
            <option value="GET">GET(select)</option>
            <option value="POST">POST(insert)</option>
            <option value="PUT">PUT(update)</option>
            <option value="DELETE">DELETE(delete)</option>
        </select>
    </p>
    <hr>
    <p id="p_evtSeq">
        경조사 시퀀스 : <input type="number" name="evtSeq" value="" />
    </p>
    <p id="p_evtBoardPage">
        조회 페이지 : <input type="number" name="evtBoardPage" value="" />
    </p>
    <p>
        <select id="p_evtPayType" name="payType">
            <option value="ALL">모두</option>
            <option value="OUTGO">지출</option>
            <option value="INCOME">수입</option>
        </select>
    </p>
    <p id="p_evtDate">
        경조사 일자 : <input type="text" name="evtDate" value=""
            placeholder="yyyy-mm-dd hh:MM:ss" />
    </p>
    <p id="p_evtTgtUserName">
        경조사 대상자 이름 : <input type="text" name="evtTgtUserName" value="동길홍" />
    </p>
    <p id="p_evtTgtUserPhoneNum">
        경조사 대상자 폰번 : <input type="text" name="evtTgtUserPhoneNum"
            value="010-0123-4567" />
    </p>
    <p id="p_evtTypeCode">
        경조사 종류 : <input type="number" name="evtTypeCode" value="1" />
    </p>
    <p id="p_relationTypeCode">
        대상과의 관계 : <input type="number" name="relationTypeCode" value="11" />
    </p>
    <p id="p_payAmount">
        지출 금액 : <input type="number" name="payAmount" value=""
            placeholder="100000" />
    </p>
    <p id="p_evtMemo">
        메모 : <input type="text" name="evtMemo" value="" />
    </p>
    <p>
        <input type="button" name="submit" style="width: 300px; height: 50px;"
            value="전송" onclick="submit('false')" />
    </p>
    <hr>
    <label>전송데이터 :</label>
    <div class="textwrapper">
        <textarea style="width: 99%; height: 150px" name="sendData" readonly></textarea>
    </div>
    <label>응답데이터 :</label>
    <div class="textwrapper">
        <textarea style="width: 99%; height: 150px" name="recvData" readonly></textarea>
    </div>
    <hr>
</body>
<script>
    // GET, POST, PUT, DELETE 라디오 변경 시
    $('#methodSelect').change(function() {
        hideAllInput();
        switch ($(this).val()) {
        case 'GET':
            $("#p_evtBoardPage").show();
            $("#p_evtPayType").show();
            break;
        case 'PUT':
            $("#p_evtSeq").show();
        case 'POST':
            $("#p_evtDate").show();
            $("input[name='evtDate']").val(getTimeStamp());
            $("#p_evtTgtUserName").show();
            $("#p_evtTgtUserPhoneNum").show();
            $("#p_evtTypeCode").show();
            $("#p_relationTypeCode").show();
            $("#p_payAmount").show();
            $("#p_evtMemo").show();
            break;
        case 'DELETE':
            $("#p_evtSeq").show();
            break;
        }
    });
</script>
<script>
    // 초기화
    hideAllInput();
</script>
</html>