package main;import java.io.File;
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
public class Main {
    int N, M, T;
    MachineList machines = new MachineList();
    VirtualMachineList virtualMachines = new VirtualMachineList();

    MachineList realMachines = new MachineList();
    TreeMap<Integer, VirtualMachine> realVirtualMachines = new TreeMap<>();
    TreeMap<Integer, Integer> belong = new TreeMap<>();
    TreeMap<Integer, Integer> config = new TreeMap<>();
    Scanner sc, ans;
    long totalCost = 0, pre = 0;

    private long solveOneDay(){
        long cost = 0;
        String s = ans.nextLine();
        String[] l = s.replace('(', ' ').replace(')', ' ').trim().split(",");
        int Q = Integer.parseInt(l[1].trim());
//        System.out.println(Q);
        for(int i=0;i<Q;i++){
            s = ans.nextLine();
            l = s.replace('(',' ').replace(')',' ').trim().split(",");
            int t = Integer.parseInt(l[1].trim());
            pre += t;
            for(int j=0;j<t;j++){
                realMachines.append(machines.getByName(l[0]).clone());
                cost += machines.getByName(l[0]).p;
            }
        }
//        System.out.println(pre + " " + realMachines.size());
        s = ans.nextLine();
//        int W = Integer.parseInt(s.trim());
//        if(W != 0){System.exit(-1);}

        int R = Integer.parseInt(sc.nextLine().trim());
        for(int i=0;i<R;i++){
            String _s = sc.nextLine();

//            System.out.println(i+" "+_s);
            l = _s.replace('(',' ').replace(')',' ').trim().split(",");
            for(int j=0;j<l.length;j++)l[j] = l[j].trim();
            if(l[0].equals("del")){
                int id = Integer.parseInt(l[1]);
//                System.out.println("id = " + id);
//                System.out.println(belong.keySet());
                int mid = belong.get(id);
                if(config.get(id) == 1){
                    realMachines.get(mid).coreB += realVirtualMachines.get(id).core;
                    realMachines.get(mid).mmB += realVirtualMachines.get(id).mm;
                }else if(config.get(id) == 2){
                    realMachines.get(mid).coreA += realVirtualMachines.get(id).core/2;
                    realMachines.get(mid).mmA += realVirtualMachines.get(id).mm/2;
                    realMachines.get(mid).coreB += realVirtualMachines.get(id).core/2;
                    realMachines.get(mid).mmB += realVirtualMachines.get(id).mm/2;

                }else if(config.get(id) == 0){
                    realMachines.get(mid).coreA += realVirtualMachines.get(id).core;
                    realMachines.get(mid).mmA += realVirtualMachines.get(id).mm;
                }
                belong.remove(id);
                realVirtualMachines.remove(id);
            }else if(l[0].equals("add")){
                VirtualMachine vm = virtualMachines.getByName(l[1].trim()).clone(); // ?????????????????????
                int id = Integer.parseInt(l[2].trim()); //?????????????????????id
                if(realVirtualMachines.containsKey(id))
                    realVirtualMachines.replace(id, vm); //???????????????
                else
                    realVirtualMachines.put(id, vm);
                _s = ans.nextLine();
                l = _s.replace('(',' ').replace(')',' ').trim().split(",");//??????
//                System.out.println(_s);
                int machine_id = Integer.parseInt(l[0].trim()); // ???????????????
                if(belong.containsKey(id))
                    belong.replace(id, machine_id);// ?????????????????????
                else
                    belong.put(id, machine_id);
//                System.out.println(id);
                int flag = 0;
                if(vm.db){
                    if(l.length == 2){System.out.println("???????????????????????????????????????");System.exit(-2);}
                    flag = 2;
                }else{
                    if(l.length != 2){System.out.print("??????????????????????????????????????? " + vm.toString() + l[0]+" "+l[1]);System.exit(-3);}
                    flag = (l[1].trim().equals("A") ? 0 : 1); // ??????AB?????????
                }
                if(config.containsKey(id))
                config.replace(id, flag);
                else
                    config.put(id, flag);
                if(flag == 2){
                    realMachines.get(machine_id).coreA -= vm.core/2;
                    realMachines.get(machine_id).coreB -= vm.core/2;
                    realMachines.get(machine_id).mmA -= vm.mm/2 ;
                    realMachines.get(machine_id).mmB -= vm.mm/2;
                }else if(flag == 0){
                    realMachines.get(machine_id).coreA -= vm.core;
                    realMachines.get(machine_id).mmA -= vm.mm;
                }else if(1 == flag){
                    realMachines.get(machine_id).coreB -= vm.core;
                    realMachines.get(machine_id).mmB -= vm.mm;
                }
                Machine tmp = realMachines.get(machine_id);
                if(tmp.coreA<0 || tmp.coreB<0 || tmp.mmA<0 || tmp.mmB<0) {
                    System.out.println("????????????????????????\n" + machine_id + "\n" + tmp);
                    System.exit(-5);

                }
            }else{
                System.out.println("??????????????????");
                System.exit(-1);
            }
        }
        boolean[] vis = new boolean[realMachines.size()];
//        System.out.println(realVirtualMachines.keySet());
//        System.out.println(machines.size() + " " + belong);
        for(Integer vm : realVirtualMachines.keySet()){

            vis[belong.get(vm)] = true;
        }
        for(int i=0;i<realMachines.size();i++){
            if(vis[i]){
                cost += realMachines.get(i).cost;
            }
        }
//        System.out.println("machine size :" + realMachines.size());
        return cost;
    }
    private void readData(){
        N = sc.nextInt();
        sc.nextLine();
        for(int i=0;i<N;i++)machines.append(sc.nextLine());
//        for(int i=0;i<N;i++)System.out.println(machines.get(i));
        M = sc.nextInt();
        sc.nextLine();
        for(int i=0;i<M;i++)virtualMachines.append(sc.nextLine());
//        for(int i=0;i<M;i++)System.out.println(virtualMachines.get(i));
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
