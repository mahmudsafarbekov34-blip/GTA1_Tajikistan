# Multiplayer

## Вазъияти ҳозира

Multiplayer ҳоло **амалӣ ё санҷида нашудааст**. Repository ҳанӯз Unreal compilation надорад.

## LAN — тарҳи баъдӣ

- Unreal replication бо host/server-authority; LAN session discovery ва join.
- Ҳадафи аввал: 2 бозингар, баъд load test то 10 бозингар.
- Санҷиш бо дастгоҳҳои воқеӣ дар ҳамон Wi‑Fi/hotspot; санҷиши firewall, reconnect, номи бозингар ва replication-и мошин.
- Интернет барои LAN session талаб нашавад. Ин тарҳ бояд аз backend-и online ҷудо бошад.

## Online — тарҳи баъдӣ

Online dedicated server, lobby ва matchmaking марҳилаи алоҳидаанд. Render Free Web Service барои сервери dedicated-и бозӣ кафолати 24/7 намедиҳад: хидмат пас аз 15 дақиқаи traffic-и воридотӣ нест шуданаш мумкин аст, файлҳои local ephemeral мебошанд ва танҳо як HTTP port-и public forward мешавад. Private service метавонад протоколҳои гуногунро дар private network қабул кунад, аммо он аз интернет public намебошад ва free plan-ро дастгирӣ намекунад.

Барои online multiplayer бояд протоколи Unreal, талаботи UDP/TCP, hosting, latency ва хароҷот пешакӣ санҷида шаванд. Ҳеҷ Render service сохта нашудааст ва ягон хизматрасонии пулакӣ фаъол карда нашудааст.

Манбаъҳо: [Render Free](https://render.com/docs/free), [Render Web Services](https://render.com/docs/web-services), [Render Private Services](https://render.com/docs/private-services).
