/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Buffer
*/

#include "server_config.h"
#include <unistd.h>
#include <string.h>

int buffer_read(client_t *client)
{
    int available = BUFFER_SIZE - client->read_len;
    int bytes = read(client->fd,
        client->read_buf + client->read_len, available);

    if (bytes <= 0)
        return -1;
    client->read_len += bytes;
    return bytes;
}

int buffer_write(client_t *client)
{
    int bytes = write(client->fd,
        client->write_buf, client->write_len);

    if (bytes <= 0)
        return -1;
    client->write_len -= bytes;
    memmove(client->write_buf,
        client->write_buf + bytes, client->write_len);
    return bytes;
}

char *buffer_get_line(client_t *client)
{
    int cmd_len = 0;
    char *newline = memchr(client->read_buf, '\n', client->read_len);
    int len = 0;

    if (newline == NULL)
        return NULL;
    len = newline - client->read_buf;
    cmd_len = len;
    if (cmd_len > 0 && client->read_buf[cmd_len - 1] == '\r') {
        cmd_len--;
    }
    memcpy(client->line_buf, client->read_buf, cmd_len);
    client->line_buf[cmd_len] = '\0';
    client->read_len -= (len + 1);
    memmove(client->read_buf, client->read_buf + len + 1, client->read_len);
    return client->line_buf;
}
