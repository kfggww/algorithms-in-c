import gdb
import os


class PrintRBT(gdb.Command):
    def __init__(self):
        super().__init__("print-rbt", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        root = gdb.parse_and_eval(arg)
        q = [root]
        nill_id = 0

        dot_cmd = "digraph {"
        while len(q) > 0:
            node = q.pop(0)
            node_val = str(node["value"])

            if node["left"] != 0:
                node_left = node["left"].dereference()
                node_left_val = node_left["value"]
                dot_cmd += "{}->{};".format(node_val, node_left_val)
                if node_left["color"] == 1:
                    dot_cmd += "{}[color=red];".format(node_left_val)
                q.append(node_left)
            else:
                dot_cmd += "nill{}[shape=point];".format(nill_id)
                dot_cmd += "{}->nill{};".format(node_val, nill_id)
                nill_id += 1

            if node["right"] != 0:
                node_right = node["right"].dereference()
                node_right_val = node_right["value"]
                dot_cmd += "{}->{};".format(node_val, node_right_val)
                if node_right["color"] == 1:
                    dot_cmd += "{}[color=red];".format(node_right_val)
                q.append(node_right)
            else:
                dot_cmd += "nill{}[shape=point];".format(nill_id)
                dot_cmd += "{}->nill{};".format(node_val, nill_id)
                nill_id += 1

        dot_cmd += "}"

        with open("/tmp/rbt.dot", "w") as f:
            f.write(dot_cmd)

        os.system("dot -Tsvg /tmp/rbt.dot > /tmp/rbt.svg")
        os.system("xdg-open /tmp/rbt.svg")


PrintRBT()
