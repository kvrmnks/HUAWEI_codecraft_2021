import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.lang.reflect.Array;
import java.util.*;
class Machine{
    String name;
    int coreA,coreB, mmA, mmB, p, cost;
    public Machine(String s) {
        s = s.replace('(', ' ');
        s = s.replace(')', ' ');
        s = s.trim();
        String[] list = s.split(",");
        for (int i = 0; i < list.length; i++) list[i] = list[i].trim();
        name = list[0];
        coreA = Integer.parseInt(list[1])/2; coreB = coreA;
        mmA = Integer.parseInt(list[2])/2; mmB = mmA;
        p = Integer.parseInt(list[3]);
        cost = Integer.parseInt(list[4]);
    }
    private Machine(){}
    public Machine clone(){
        Machine a = new Machine();
        a.name = this.name;
        a.coreA = this.coreA;
        a.coreB = this.coreB;
        a.mmA = this.mmA;
        a.mmB = this.mmB;
        a.p = this.p;
        a.cost = this.cost;
        return a;
    }

    @Override
    public String toString() {
        return "Machine{" +
                "name='" + name + '\'' +
                ", coreA=" + coreA +
                ", coreB=" + coreB +
                ", mmA=" + mmA +
                ", mmB=" + mmB +
                ", p=" + p +
                ", cost=" + cost +
                '}';
    }
}
class MachineList{
    public ArrayList<Machine> data = new ArrayList<>();
    public void append(Machine x){data.add(x);}
    public Machine getByName(String s){
        for(int i=0;i<data.size();i++)if(data.get(i).name.equals(s))return data.get(i);
        return null;
    }
    public void append(String s){
        data.add(new Machine(s));
    }
    public Machine get(int x){
        return data.get(x);
    }
    public int size(){
        return data.size();
    }
}
class VirtualMachine{
    String name;
    int core, mm;
    boolean db;
    private VirtualMachine(){}
    public VirtualMachine(String s){
        s = s.replace('(', ' ');
        s = s.replace(')', ' ');
        s = s.trim();
        String[] list = s.split(",");
        for (int i = 0; i < list.length; i++) list[i] = list[i].trim();
        name = list[0];
        core = Integer.parseInt(list[1]);
        mm = Integer.parseInt(list[2]);
        db = (Integer.parseInt(list[3]) > 0);
    }
    public VirtualMachine clone(){
        VirtualMachine ret = new VirtualMachine();
        ret.name = this.name;
        ret.core = this.core;
        ret.mm = this.mm;
        ret.db = this.db;
        return ret;
    }
    @Override
    public String toString() {
        return "VirtualMachine{" +
                "name='" + name + '\'' +
                ", core=" + core +
                ", mm=" + mm +
                ", db=" + db +
                '}';
    }
}
class VirtualMachineList{
    public ArrayList<VirtualMachine> data = new ArrayList<>();
    public void append(VirtualMachine x){data.add(x);}
    public void append(String s){
        data.add(new VirtualMachine(s));
    }
    public VirtualMachine getByName(String s){
        for(int i=0;i<data.size();i++)if(data.get(i).name.equals(s))return data.get(i);
        return null;
    }
    public VirtualMachine get(int x){
        return data.get(x);
    }
    public int size(){
        return data.size();
    }
}
class Action{
    MachineList realMachines = new MachineList();
    TreeMap<Integer, VirtualMachine> realVirtualMachines = new TreeMap<>();
    TreeMap<Integer, Integer> belong = new TreeMap<>();
    TreeMap<Integer, Integer> config = new TreeMap<>();
    public void addMachine(Machine x){
        realMachines.append(x);
    }
    public void delVirtualMachine(int vm_id){
        int machine_id = belong.get(vm_id);
        VirtualMachine vm = realVirtualMachines.get(vm_id);
        if(config.get(vm_id) == 1){
            realMachines.get(machine_id).coreB += vm.core;
            realMachines.get(machine_id).mmB += vm.mm;
        }else if(config.get(vm_id) == 2){
            realMachines.get(machine_id).coreA += vm.core/2;
            realMachines.get(machine_id).mmA += vm.mm/2;
            realMachines.get(machine_id).coreB += vm.core/2;
            realMachines.get(machine_id).mmB += vm.mm/2;

        }else if(config.get(vm_id) == 0){
            realMachines.get(machine_id).coreA += vm.core;
            realMachines.get(machine_id).mmA += vm.mm;
        }
        belong.remove(vm_id);
        config.remove(vm_id);
        realVirtualMachines.remove(vm_id);
    }
    public void addVirtualMachine(int vm_id, int machine_id, int cf, VirtualMachine vm){
//        System.out.println(cf);
//        for(int x : belong.keySet()){
//            if(belong.get(x) == 22){
//                System.out.println(realVirtualMachines.get(x) + " node: " + config.get(x) + " id: " + x);
//            }
//        }
        if(belong.containsKey(vm_id)){
            belong.replace(vm_id, machine_id);
        }else{
            belong.put(vm_id, machine_id);
        }
        if(config.containsKey(vm_id)){
            config.replace(vm_id, cf);
        }else{
            config.put(vm_id, cf);
        }
        if(realVirtualMachines.containsKey(vm_id)){
            realVirtualMachines.replace(vm_id, vm);
        }else{
            realVirtualMachines.put(vm_id, vm);
        }
        if(config.get(vm_id) == 1){
            realMachines.get(machine_id).coreB -= vm.core;
            realMachines.get(machine_id).mmB -= vm.mm;
        }else if(config.get(vm_id) == 2){
            realMachines.get(machine_id).coreA -= vm.core/2;
            realMachines.get(machine_id).mmA -= vm.mm/2;
            realMachines.get(machine_id).coreB -= vm.core/2;
            realMachines.get(machine_id).mmB -= vm.mm/2;

        }else if(config.get(vm_id) == 0){
            realMachines.get(machine_id).coreA -= vm.core;
            realMachines.get(machine_id).mmA -= vm.mm;
        }
        Machine tmp = realMachines.get(machine_id);
        if(tmp.coreA<0 || tmp.coreB<0 || tmp.mmA<0 || tmp.mmB<0) {

                    for(int x : belong.keySet()){
            if(belong.get(x) == 23){
                System.out.println(realVirtualMachines.get(x) + " node: " + config.get(x) + " id: " + x);
            }
        }
            System.out.println("服务器的资源溢出\n" + machine_id + "\n" + tmp);
            System.exit(-5);

        }
    }
    public long getCost(){
        long cost = 0;
        boolean[] vis = new boolean[realMachines.size()];
        for(Integer vm : realVirtualMachines.keySet()){

            vis[belong.get(vm)] = true;
        }
        for(int i=0;i<realMachines.size();i++){
            if(vis[i]){
                cost += realMachines.get(i).cost;
            }
        }
        return cost;
    }
    public VirtualMachine getVm(int vm_id){
        return realVirtualMachines.get(vm_id);
    }
}
public class Main {
    int N, M, T;
    MachineList machines = new MachineList();
    VirtualMachineList virtualMachines = new VirtualMachineList();
    Action action = new Action();
    Scanner sc, ans;
    long totalCost = 0, pre = 0;

    private long solveOneDay(){
        long cost = 0;
        String s = ans.nextLine();
        String[] l = s.replace('(', ' ').replace(')', ' ').trim().split(",");
        int Q = Integer.parseInt(l[1].trim());
        for(int i=0;i<Q;i++){
            s = ans.nextLine();
            l = s.replace('(',' ').replace(')',' ').trim().split(",");
            int t = Integer.parseInt(l[1].trim());
            pre += t;
            for(int j=0;j<t;j++){
                action.addMachine(machines.getByName(l[0]).clone());
                cost += machines.getByName(l[0]).p;
            }
        }
        s = ans.nextLine();
        l = s.replace('(',' ').replace(')', ' ').trim().split(",");
        int M = Integer.parseInt(l[1].trim());
        for(int i=0;i<M;i++){
            s = ans.nextLine();
            l = s.replace('(',' ').replace(')', ' ').trim().split(",");
            int vm_id = Integer.parseInt(l[0].trim());
            int new_machine_id = Integer.parseInt(l[1].trim());
            VirtualMachine vm = action.getVm(vm_id);
            action.delVirtualMachine(vm_id);
            int nodeId = 2;
            if(l.length == 3){
                nodeId = (l[2].trim().equals("A")?0:1);
                if(vm.db){
                    System.out.println("错误的迁移方案，将双节点单部署");
                    System.exit(-6);
                }
            }else{
                if(!vm.db){
                    System.out.println("错误的迁移方案，将单节点双部署");
                    System.exit(-6);
                }
            }

            action.addVirtualMachine(vm_id, new_machine_id, nodeId, vm);
        }
        int R = Integer.parseInt(sc.nextLine().trim());
        for(int i=0;i<R;i++){
            String _s = sc.nextLine();
            l = _s.replace('(',' ').replace(')',' ').trim().split(",");
            for(int j=0;j<l.length;j++)l[j] = l[j].trim();
            if(l[0].equals("del")){
                int id = Integer.parseInt(l[1]);
                action.delVirtualMachine(id);
            }else if(l[0].equals("add")){
                VirtualMachine vm = virtualMachines.getByName(l[1].trim()).clone(); // 所需要的虚拟机
                int id = Integer.parseInt(l[2].trim()); //给虚拟机分配的id
                _s = ans.nextLine();
                l = _s.replace('(',' ').replace(')',' ').trim().split(",");//分配
                int machine_id = Integer.parseInt(l[0].trim()); // 服务器编号
                int flag = 0;
                if(vm.db){
                    if(l.length == 2){System.out.println("将一个双部署的虚拟机单部署");System.exit(-2);}
                    flag = 2;
                }else{
                    if(l.length != 2){System.out.print("将一个单部署的虚拟机双部署 " + vm.toString() +" "+ l[0]+" "+l[1] + " " + l[2]);System.exit(-3);}
                    flag = (l[1].trim().equals("A") ? 0 : 1); // 分配AB节点号
                }
                action.addVirtualMachine(id, machine_id, flag, vm);

            }else{
                System.out.println("参数解析错误");
                System.exit(-1);
            }
        }

        return cost + action.getCost();
    }
    private void readData(){
        N = sc.nextInt();
        sc.nextLine();
        for(int i=0;i<N;i++)machines.append(sc.nextLine());
        M = sc.nextInt();
        sc.nextLine();
        for(int i=0;i<M;i++)virtualMachines.append(sc.nextLine());
    }

    Main() throws FileNotFoundException {
        FileInputStream fileIn = new FileInputStream("./training-2.txt");
        System.setIn(fileIn);
        sc = new Scanner(System.in);
        ans = new Scanner(new FileInputStream("./training-2-out.txt"));
        readData();
        T = sc.nextInt();
        sc.nextLine();
        long cost_sum = 0;
        for(int i=0;i<T;i++){
            long cost = solveOneDay();
            cost_sum += cost;
            System.out.println("cost: "+cost + " on day:" + i);
        }
        System.out.println("totalCost: " + cost_sum);

    }
    public static void main(String[] args) throws FileNotFoundException {
        new Main();
    }
}
