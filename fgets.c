// fgetsで1行stdinから読み取るコード

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    FGETS_SUCCESS,
    FGETS_SUCCESS_EOF,
    FGETS_FAILURE,
    FGETS_FAILURE_BUF_LIMIT,
} FgetsRetval;

int
read_line(FILE *fp, char *buf, size_t buf_size)
{
    if (fp == NULL || buf == NULL) {
        return FGETS_FAILURE;
    }

    if (fgets(buf, buf_size, fp) == NULL) {
        // man fgets:
        // fgets() returns s on success, and NULL on error or when end of file occurs while no characters have been read.
        if (feof(fp)) {
            // 読み込むべき文字がない
            return FGETS_SUCCESS_EOF;
        } else {
            // fgets()失敗

            // FIO40-C. fgets() が失敗したときは引数に渡した配列の内容をリセットする
            // https://www.jpcert.or.jp/sc-rules/c-fio40-c.html
            buf[0] = '\0';
            return FGETS_FAILURE;
        }
    } else {
        // FIO36-C. fgets() が改行文字を読み取ると仮定しない
        // https://www.jpcert.or.jp/sc-rules/c-fio36-c.html
        // FIO37-C. fgets() や fgetws() が読み取り成功時に空でない文字列を返すと想定しない
        // https://www.jpcert.or.jp/sc-rules/c-fio37-c.html
        size_t len = strlen(buf);
        if (len == 0) {
            return FGETS_SUCCESS_EOF;
        }

        if (buf[len - 1] != '\n') {
            // ユーザが改行文字を入力したためfgetsが終了したのではなく,
            // バッファサイズが一杯になってfgetsが終了した.

            // ストリームに残っているゴミを吸い取る
            while (fgetc(fp) != '\n');
            return FGETS_FAILURE_BUF_LIMIT;
        }

        // 改行文字を削除
        buf[len - 1] = '\0';
        return FGETS_SUCCESS;
    }
}

int
main(int argc, char *argv[])
{
    char buf[256] = { 0 };
    FgetsRetval retval = read_line(stdin, buf, sizeof(buf));

    switch (retval) {
    case FGETS_SUCCESS:
        // fall through
    case FGETS_SUCCESS_EOF:
        printf("%s\n", buf);
        exit(EXIT_SUCCESS);
    case FGETS_FAILURE:
        fprintf(stderr, "[ERROR] ストリームに文字が残っているのにも関わらずfgetsがEOFを返却した\n");
        exit(EXIT_FAILURE);
    case FGETS_FAILURE_BUF_LIMIT:
        fprintf(stderr, "[ERROR] バッファサイズを超えて入力した\n");
        exit(EXIT_FAILURE);
    default:
        fprintf(stderr, "[BUG] エラーハンドリングの不備: retval=[%d]\n", retval);
        exit(EXIT_FAILURE);
    }
}

