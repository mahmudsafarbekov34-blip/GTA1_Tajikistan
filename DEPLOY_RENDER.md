# Render deployment

## Вазъият

Backend, Dockerfile, health endpoint ва Render deployment ҳоло вуҷуд надоранд. Ин ҳуҷҷат тарҳ ва маҳдудиятҳоро сабт мекунад; deployment иҷро нашудааст.

## Маҳдудиятҳои ройгон

Мувофиқи ҳуҷҷатҳои Render:

- Free Web Service баъди 15 дақиқаи бе inbound HTTP/WebSocket traffic spin down мешавад ва оғози дубора тақрибан як дақиқа мегирад.
- Filesystem-и хидмати free ephemeral аст; free service persistent disk гирифта наметавонад.
- Free instance hours ва bandwidth/pipeline маҳдуданд; ҳангоми тамомшавӣ service метавонад suspend шавад ё вобаста ба ҳолати billing хароҷот/лимит ба миён ояд.
- Public Web Service танҳо як HTTP port-ро аз интернет forward мекунад.
- Private Service метавонад протоколҳои дигарро дар private network истифода барад, аммо аз интернет дастрас нест ва free instance надорад.

Аз ин рӯ Render Free барои Unreal dedicated game server-и доимӣ ё нигоҳдории save файлии бозингарон мувофиқ нест. Backend-и HTTP-и идоракунӣ/health endpoint-ро аз authoritative game server ҷудо нигоҳ доштан лозим аст. Барои маълумоти пойдор database-и мувофиқ ё storage-и алоҳида лозим мешавад.

## Пеш аз deployment

- Backend ва health-check-и воқеӣ сохта, test кунед.
- Танҳо environment variable-и ғайримахфӣ дар repo нигоҳ доред; secret-ҳо дар Render dashboard.
- Docker image-ро маҳаллӣ санҷед.
- Нақша, лимити хароҷот, persistent storage, restart ва logging-ро тасдиқ кунед.
- Deployment-и пулакӣ бе иҷозати соҳиб фаъол карда нашавад.

Манбаъ: [Render Free Instance Limitations](https://render.com/docs/free), [Web Services](https://render.com/docs/web-services), [Persistent Disks](https://render.com/docs/disks).
