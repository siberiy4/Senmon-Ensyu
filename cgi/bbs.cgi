#!/usr/bin/perl

use CGI;
use CGI::Carp qw/ fatalsToBrowser /;
use JSON qw/encode_json decode_json/;
use utf8;
use Encode;
use LWP::UserAgent;

$q = new CGI;
$q->charset('utf-8');
require 'init.cgi';
if($ENV{CONTENT_LENGTH} eq ''){
&printHTML;
}else{
    @empty_text=&check_empty_text;    
    @xss_text=&check_xss_text;

    if((!@empty_text) && (!@xss_text)){
        #&saveLog;
        &saveDBData;
        &printHTML;
    }else{
        errprintHTML(\@empty_text,\@xss_text);
    }

}


#時間を取得
sub getTime{
    my ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime(time);
    $year += 1900;
    $mon++;
    my $time ="$year/$mon/$mday_$hour:$min";

    return $time;
}

sub printForm {
    my $res = <<"EOL";
            <div class="form-area">
                <div class="form-title">発言する</div>
            <form method="post" action="bbs.cgi">
                名前<input type="text" name="name" size="30" maxlength="20" required value="名無し">E-mail<input type="text" name="mail" size="30" required ><br>
                タイトル<input type="text" name="title" size="30" maxlength="20" value="タイトルなし" required ><br>
                メッセージ本文<br>
                <textarea name="text" cols="80" rows="5" minlength="1" required placeholder="入力がありません" ></textarea>
                <br>
                <br>
                すべて入力してください
                <input type="submit" value="投稿"> 
            </form>
            </div>
EOL

    return $res;
}

#DBの情報を受け取る
sub getDBData{

    my $ua = LWP::UserAgent->new;
    my $req = HTTP::Request->new( GET => '"Rest APIのサーバー"/posts' );
    my $res = $ua->request( $req );
    my $data = decode_json( $res->content );

    return $data;
}

#DBにデータを送る
sub saveDBData{
    $time=&getTime;
    $name=$q->param('name');
    $mail=$q->param('mail');
    $title=$q->param('title');
    $text=$q->param('text');

    my $uri = '"Rest APIのサーバー"/posts';

    my $json = '{	"title":"'.$title.'","name":"'.$name.'" ,"text":"'.$text.'" ,"mail":"'.$mail.'", "time":"'.$time.'"}';


    my $req = HTTP::Request->new( 'POST', $uri );
    $req->header( 'Content-Type' => 'application/json' );
    $req->content( $json );
    my $lwp = LWP::UserAgent->new;
    $lwp->request( $req );
}




sub makeRemark{
    my @Log = @_;
    my $res = << "EOL";
        <div class="information">
            <h3>$Log[0]</h3>
            <div class="spoken" >
                $Log[1] - $Log[2]
                </div>
                <p>$Log[3]
            </p>
        </div>

EOL

    return $res;
}

sub check_empty_text{
    $name=$q->param('name');
    $mail=$q->param('mail');
    $title=$q->param('title');
    $text=$q->param('text');
    @empty_list;
    if($name eq ''){
        push(@empty_list,"名前");
    }
    if($mail eq ''){
        push(@empty_list,"メールアドレス");
    }
    if($title eq ''){
        push(@empty_list,"タイトル");
    }
    if($text eq ''){
        push(@empty_list,"本文");
    }

    return @empty_list;
}

sub check_xss_text{
    @xss_list;

    if($name ne $q->escapeHTML($q->param('name'))){
        push(@xss_list,"名前");
    }
    if($mail ne $q->escapeHTML($q->param('mail'))){
        push(@xss_list,"メールアドレス");
    }
    if($title ne $q->escapeHTML($q->param('title'))){
        push(@xss_list,"タイトル");
    }
    if($text ne $q->escapeHTML($q->param('text'))){
        push(@xss_list,"本文");
    }

    return @xss_list;
}

sub printHTML{
print << "EOL";
Content-type: text/html

<html>
	<head>
		<title>BBS</title>
		<meta http-equiv="Content-type" content="text/html; charset=UTF-8">
    <link rel="stylesheet" type="text/css" href="../info.css">
	</head>
	<body>
    <h1> 掲示板 </h1>
EOL


#@Logs = &getLog;
$Logs = &getDBData;

foreach (@{$Logs}) {
    print makeRemark($_->{title},$_->{time},$_->{name},$_->{text});
}

print &printForm;

print << "EOL";
	</body>
</html>
EOL

}

sub errprintHTML{
($emp,$xss)=@_;
print << "EOL";
Content-type: text/html

<html>
	<head>
		<title>BBS</title>
		<meta http-equiv="Content-type" content="text/html; charset=UTF-8">
    <link rel="stylesheet" type="text/css" href="../info.css">
	</head>
	<body>
    <h1> 掲示板 </h1>
EOL
my @empty_text=@{$emp};
if(@empty_text){ 
    foreach my $message (@empty_text){
        print " $message ";
    }
    print "が空です。\n"
}
my @xss_text=@{$xss};
if(@xss_text){ 
    foreach my $hoge (@xss_text){
        print " $hoge ";
    }
    print "が不正な入力です。\n"
}
print "正しい情報を入力してください\n";
#@Logs = &getLog;
$Logs = &getDBData;

foreach (@{$Logs}) {
    print makeRemark($_->{title},$_->{time},$_->{name},$_->{text});
}

print &printForm;

print << "EOL";
	</body>
</html>
EOL

}