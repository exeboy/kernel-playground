// here's the c file for our programm


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/inet.h>
#include <linux/fs.h>      // Required for file operations
#include <linux/slab.h>    // Required for kmalloc/kfree (memory allocation)
#include <linux/uaccess.h> // Required for copy_from_user, etc.

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Our Team");
MODULE_DESCRIPTION("Advanced: Drops HTTP packets based on a blacklist from /etc/http_logger_blacklist.conf");

#define MAX_BLACKLIST_SIZE 128
#define BLACKLIST_FILE_PATH "/etc/http_logger_blacklist.conf"

static __be32 blocked_ips[MAX_BLACKLIST_SIZE];
static int num_blocked_ips = 0;

static struct nf_hook_ops netfilter_ops;

unsigned int main_hook_function(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);

static int load_blacklist(void) {
    struct file *file;
    char line[32];
    int line_len;
    loff_t offset = 0;
    int i = 0;

    printk(KERN_INFO "HTTP Logger: Loading blacklist from %s\n", BLACKLIST_FILE_PATH);
    file = filp_open(BLACKLIST_FILE_PATH, O_RDONLY, 0);
    if (IS_ERR(file)) {
        printk(KERN_ERR "HTTP Logger: Failed to open blacklist file. Error %ld\n", PTR_ERR(file));
        return PTR_ERR(file);
    }

    while ((line_len = kernel_read(file, line, sizeof(line) - 1, &offset)) > 0 && i < MAX_BLACKLIST_SIZE) {
        line[line_len] = '\0'; // Null-terminate the string
        if (line[line_len - 1] == '\n') {
            line[line_len - 1] = '\0'; // Remove trailing newline
        }
        if (strlen(line) > 0) {
            blocked_ips[i] = in_aton(line);
            printk(KERN_INFO "HTTP Logger: Added %s to blacklist.\n", line);
            i++;
        }
    }
    num_blocked_ips = i;

    filp_close(file, NULL);
    printk(KERN_INFO "HTTP Logger: Loaded %d IPs into blacklist.\n", num_blocked_ips);
    return 0;
}

unsigned int main_hook_function(void *priv,
                                struct sk_buff *skb,
                                const struct nf_hook_state *state) {
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;
    int i;

    if (!skb) return NF_ACCEPT;
    ip_header = ip_hdr(skb);
    if (!ip_header) return NF_ACCEPT;

    if (ip_header->protocol == IPPROTO_TCP) {
        tcp_header = (struct tcphdr *)((__u8 *)ip_header + (ip_header->ihl * 4));

        if (ntohs(tcp_header->dest) == 80) {
            for (i = 0; i < num_blocked_ips; i++) {
                if (ip_header->saddr == blocked_ips[i]) {
                    printk(KERN_ALERT "DROPPING HTTP packet from blocked IP: %pI4\n", &ip_header->saddr);
                    return NF_DROP;
                }
            }
        }
    }
    return NF_ACCEPT;
}

static int __init http_logger_init(void) {
    printk(KERN_INFO "HTTP Logger (Advanced): Module loaded.\n");

    if (load_blacklist() != 0) {
        printk(KERN_ERR "HTTP Logger: Could not load blacklist, aborting module load.\n");
        return -EFAULT; // Error fault
    }

    netfilter_ops.hook     = main_hook_function;
    netfilter_ops.pf       = PF_INET;
    netfilter_ops.hooknum  = NF_INET_PRE_ROUTING;
    netfilter_ops.priority = NF_IP_PRI_FIRST;
    nf_register_net_hook(&init_net, &netfilter_ops);
    printk(KERN_INFO "HTTP Logger (Advanced): Netfilter hook registered.\n");
    return 0;
}

static void __exit http_logger_exit(void) {
    nf_unregister_net_hook(&init_net, &netfilter_ops);
    printk(KERN_INFO "HTTP Logger (Advanced): Netfilter hook unregistered.\n");
    printk(KERN_INFO "HTTP Logger (Advanced): Module unloaded.\n");
}

module_init(http_logger_init);
module_exit(http_logger_exit);
